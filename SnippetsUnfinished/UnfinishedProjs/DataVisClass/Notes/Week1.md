# Places to Find Good Examples
Look at ~Minute 25:00. Some examples
- https://graphics.reuters.com/
- https://www.nytimes.com/section/upshot
- https://pudding.cool/
- https://fivethirtyeight.com/
- https://flowingdata.com/
- https://www.reddit.com/r/dataisbeautiful/

And so forth...

# Assignment: find data visualizations (i.e. plan/design what to display).
## Sex Patterns at MIT
This is for a "pamphlet" that I'm making (it's actually a site) with the purpose of visualizing the sexual health and habits of people at MIT. It's based on this survey: https://thetech.com/2009/10/30/survey-v129-n49.

This will be a static site, though I want to put google ads on it. If I can, I will also try to load it from a database to learn how to integrate some basic backend. The database has to be in NodeJS for the purposes of learning to use it. If I do load from a database, the database will be created mostly programatically by using Python. Then, it will be updated manually. I will simply load the data from an endpoint. I do not intend to actually do anything interactive here (i.e. only reads, no writes). I will NOT use an SQL database in all likelihood.

### Plots
#### Virginity (Stacked) Bar Chart
Should be able to be toggled by
- Nothing (i.e. net rate)... **I think I will also have a net pie chart to the left of this**
- Major
- Year
- Gender
- FSILG vs. Dorm
- Specific Living Groups (with averaging and lumping to account for lack of data)
- Sexual Orientation

#### Virginity by Year Streamgraph
I want this as a `streamgraph` or `area graph`. Check examples of the types of graphs I'd like to make here: https://en.wikipedia.org/wiki/Streamgraph and https://observablehq.com/@d3/streamgraph-transitions. We can use exponential smoothing to turn the rather imprecise data that I have (year granularity) into something more organic-looking.

#### Virginity Area Map
This will simply be by FSLIG and will be a interactive (you can zoom in, zoom out, move around, and highlight by hovering, which should cause the area to pulse if there is heat... the pulsing will only occur for the heat corresponding to the area which you are selecting, which will depend on where your mouse is closest to) heatmap.

#### Pie Chart for Various Sexual Habits
Similar from the google form. This will include
- Frequency of sex
- Usage of Protection (toggleable between different types of sex)
- STD Testing and Emergency Contraception rates (toggleable by major, FSILG vs. Dorm, and specific living groups with appropriate lumping)
- Sex while intoxicated (toggleable by major, FSILG vs. Dorm, and specific living groups with appropriate lumping)
- Floorcest (toggleable by specific living groups)
- **Masturbation Statistics** (this one is rather important and interesting for me). We will have this toggleable by year, major, FSILG vs. Dorm, specific living groups, gender, and sexual orientation. **We will also have frequency** which will be toggleably by the exact same things. Right next to this we will have sex toy stats which will be toggleable by the exact same things.

I may or may not choose to do unions for interesting finds. For that I might also just put it in text boxes.

I can put **information about whether people know where to find condoms in a text box**. **Non-consensual sex can also be reported in a text-box** with appropriate resource links there.

#### Relationship and Motivation Charts
1. Top quotes from why people choose to have sex
2. Top quotes for influences on quality of sex
3. Distribution (smoothed perhaps with an exponential smoothing) of quality of sex, should be toggleable by major, year, gender, FSILG vs. Dorm, specific living group, and sexual orientation. When we toggle the net distribution should not go away completely, but show somewhat faded in the background.
4. Statistics on having a relationship (toggleable by major, year, gender, FSILG vs. Dorm, specific living groups, sexual orientation)
5. Statistics on dating apps toggleable by the same as above
6. List of non-MIT romantic interests by frequency (bar chart, top to bottom), color-coded by location and highlightable by that (i.e. the rest would fade into the background slightly, like in the data vis first 30 minutes examples)

#### Crazy Stories
Quote table-like section (not really a table, maybe just quotes offset in alternating patterns like you would see in a website with so-called testimonials) of crazy stories.

#### Reflections on This Survey
What we could have asked
1. When people were having sex
2. Where people were having sex
3. ...

Comparison on specific numbers to https://thetech.com/2009/10/30/survey-v129-n49, or perhaps aggregate interpretation. The specific numbers might be inlined in each of the sections.

Opinions on masturabation and commentary on society in general and my experience.

## Senior Results from MIT
This is for some statistics I want to do to understand where everyone from MIT is going, etcetera.

The main things I want to know are
1. Where is everyone located
2. Where is everyone studying or working
3. Senior Quote
4. Optionally, Highest high and lowest low
5. Optionally, Groups (Clubs, FSILGs, Sports Teams, etcetera...) that you affiliate yourself with

For this I think I will just create
1. Rotatable globe of the world with heatmap of people
2. List of companies (bar chart, color-coded by major and toggleable by that or gender), universities, and other "things to do"
3. List of quotes, with standouts at the top
4. List of highest highs and lowest lows that I found useful

I will also add a form for whether people want to stay connected in some way. If possible, I want a low friction way of knowing who knows who so I can use this information in the future. However, that's not so crucial and is instead more of a focus of my CRM (below).

## My Friends Groups and Locations (from MIT and beyond)
Ideas are a still a WIP. Check https://docs.google.com/document/d/1Lywu5C3o7nNDmX2u-Kf82oEx--vCULpHzzDY1bjZ4Yw/edit for more.

This is for a CRM that I want to build in React or React Native. Using React Native could be a good experience for me since it will be something different from just pure react. I might build both. Whatever the case, **for this one, I DO want a BACKEND with login/logoff, including using OAUTH for Google**.

1. Map of where my friends are
2. Table with tags for what groups they belong to
3. Table should also have tags for who is working where
4. Table should also have information for who knows who (which will correspond to a graph later on)
5. Table should  
6. Searchable graph for who knows who

If possible, I'd love the CRM to include functionality to automatically send out
1. Mass messenger mails **with templates**
2. Mass emails (similar)
3. Mass text (similar)

It would also be cool to have it store
1. A list of icebreakers and recommendations for different situations (maybe I can have some sort of query language to describe the situation I am going into and it can me suggestions)
2. Information pertinent to specific situations (i.e. if I'm going to a sports game, it can give me a set of facts I should understand to be able to vibe with the people there and understand their values and their likes and dislikes)
3. A list of events that might be pertinent for people, or a list of who to invite to events (with algorithms to improve their experience, like for example making sure they have both people they know and people they don't know)
4. Have something like a rating system or ordering system, or at least a "cache," to know who I am hottest with right now
5. Connection-maker and/or mutual benefit-contacter (i.e. when person A needs X and person B gives X, but they don't know each other, connect them)
6. Keeping track of birthdays and what people like
7. Some sort of tool to make sure I remember the names, likes, birthdays and other relevant metadata for friends I really care the most about. I don't want everything to be automated.
8. List of who's good at what so I can have a "personal board of advisors." Might also want to set up some sort of mailing list. Who knows.

Also some crazy automation ideas
1. Automatically like and comment on posts
2. Automatically post life updates multi-modally

It is important to look at the tools recomended by Dave Kobrosky as of December 2021 (Zapier, Twilio, clay.run, clay.earth, Coda, Airtable, Folk, ...). Also, what about Clay or existing solutions? What do they give me or not give me that I want?