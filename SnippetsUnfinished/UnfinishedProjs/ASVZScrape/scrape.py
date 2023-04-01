from __future__ import annotations
from typing import Optional, Union
import requests
import json
import pathlib
from datetime import datetime, timedelta
import shutil
from pprint import pprint

def generate_listings_ep(query_limit: int, query_offset: int) -> str:
    """
    Generate a listings endpoint string that can be used by `scrape`
    (defined below) so as to query ASVZ (using the requests library).
    
    Params:
        query_limit (int) - how many event entries to get back
        query_offset (int) - used to use low query limits with
            multiple requests (i.e. first you might get the first
            100 events, then you'll want the ones after the first
            100, the offset tells you how many in the beginning
            to ignore)
    """
    return ('https://www.asvz.ch/asvz_api/event_search?_format=json' +
        f'&limit={query_limit}' +
        f'&offset={query_offset}')

def scrape(
    verbose: bool = True,
    save_scrape: bool = True,
    clear_prev_scrape: bool = True) -> None:
    """
    Scrape the latest listings from ASVZ and return them.

    Params:
        verbose (bool) - whether to print information about the
            status of scraping as it happens.
        save_scrape (bool) - whether to save the scrape (json of
            listings) to the .cache folder located next to this
            python file.
        clear_prev_scrape (bool) - whether to delete the .cache
            folder next to this script before performing the scrape.
    """
    if clear_prev_scrape:
        if pathlib.Path('.cache').exists():
            shutil.rmtree('.cache')
    
    # While there are more listings to scrape, add them to our list of listings
    # (assume that there are not so many that we'll run out of memory)
    # Relevant content internals (some that may be worth looking into later):
    # - ['state']['utc_offset']
    # - ['facets'] with the queryId parameters provided for each facet
    # - ['count']['total']
    # - ['count']['offset']
    # - ['results']
    listings = []
    content_remains = True
    query_limit = 1000
    query_offset = 0
    total = None
    while content_remains:
        listings_ep = generate_listings_ep(query_limit, query_offset)
        resp = requests.get(listings_ep)
        if resp.status_code != 200:
            raise RuntimeError(f'Faild to get content, status {resp.status_code}')
        content = json.loads(resp.content)

        # Update the total
        if total:
            if tot := content['count']['total'] != total:
                raise ValueError(f'Total number of events changed from {total} to {tot}')
        else:
            total = content['count']['total']
        
        # Print informative output
        if verbose:
            print(f'Scraped {min(query_offset + query_limit, total)}/{total}')
        
        # Update the offset
        if off := content['count']['offset'] !=  query_offset:
            raise RuntimeError(f'Offset should have been {query_offset} but was {off}')
        if limit := content['count']['limit'] != query_limit:
            raise RuntimeError(f'Limit should have been {query_limit} but was {limit}')
        if query_offset + query_limit >= total:
            content_remains = False
        else:
            query_offset += query_limit

        # Update our list of listings
        listings.append(content['results'])
    
    if save_scrape:
        # Second-level accuracy should be OK because
        # we usually will not have more than one call per minute
        # (since the whole process takes a couple seconds)
        now = datetime.now().strftime('%Y-%m-%d_%H_$M_%S')
        cache_path = pathlib.Path('.cache')
        if not cache_path.exists():
            cache_path.mkdir()
        with open(f'.cache/scrape_raw_{now}.json', 'w') as scrape_raw_f:
            json.dump(listings, scrape_raw_f, indent = 4)

    return listings

def find_events(
    listings: Union[None, str, list],
    start: datetime,
    stop: datetime,
    exclude: Optional[list[tuple[datetime, datetime]]] = None,
    buffer: Optional[timedelta] = timedelta(minutes=30)):
    """
    Given a listings list (that could be from .cache, a path to a json, or
    an actual json decoded into a list of dicts), as well as start and end times
    between which to find events, find events that are possible to do.
    
    Params:
        listings
        start
        stop
        exclude
        buffer
    """
    pass


if __name__ == '__main__':
    listings = scrape(verbose = True, save_scrape = True, clear_prev_scrape = True)
    # XXX Format them into a pretty calendar-like table based on start-end times
    # XXX Remove the ones that are on no-go times
    # XXX Display the result
    pprint(listings[0])