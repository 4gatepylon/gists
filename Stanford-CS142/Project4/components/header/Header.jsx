import React from 'react';
import './Header.css'
import { CircularProgress } from '@mui/material';
import { GenerateOkPermutations } from '../../helpers/Permutations';
import { isInt } from '../../helpers/Misc'

// XXX add circular loading
// XXX bug with input showing wrong sometimes
// XXX deduplicate permutations

export default class Header extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            n: 0,
            maxN: 4,
            error: '',
            permutationsCache: {},
            permutations: [],
            loadingPermutations: false
        }
        this.handleNChanger = this.handleNChanger.bind(this)
    }

    handleNChanger (e) {
        console.log(this.state.permutationsCache) // XXX this should be changing!
        // Maybe can be made prettier? TODO
        const _newN = e.target.value;
        const maxN = this.state.maxN
        const isIn = isInt(_newN)
        const newN = isIn ? parseInt(_newN) : _newN;
        let newState = {n : newN}
        if (!(isIn && newN <= maxN)) {
            // Would be nicer to try and do this with fadein/fadeout
            // Look at options like this:
            // https://stackoverflow.com/questions/41965978/react-styled-components-fade-in-fade-out
            newState = {
                ...newState,
                error: (
                    isIn ? `Picked n to be too high (${this.state.n}), max allowable is ${maxN}` : 
                    newN.length > 0 ? `Please pick a low integer!` : ''
                )
            }
        } else {
            newState = {
                ...newState,
                permutations: this.getNewPermutations(newN)
            }
        }
        this.setState(newState)
    }

    getNewPermutations(n) {
        const permutationsCache = this.state.permutationsCache
        if (!(isInt(n) && n <= this.state.maxN) || n <= 0) return []
        else if (permutationsCache[n] !== undefined) return permutationsCache[n]
        else {
            // Can take up to 5 seconds on the first run for 4x4 (brute force...)
            const ans = GenerateOkPermutations(n);
            this.setState({permutationsCache : {
                ...permutationsCache,
                [n]: ans
            }})
            return ans;
        }
    }

    render() {
        const error = this.state.error;
        const n = this.state.n;
        const permutations = this.state.permutations;
        return (
        <div className="nav flexcol">
            <a href="/"><img style={{width: "40px", height:"40px"}} src="../../assets/home.png"/></a>
            <h3>
            Welcome to Adriano's CS142 website! Enter below to get a list of the possible finite groups!
            </h3>
            <label htmlFor="nChanger">What size : </label>
            <input 
                id="nChanger"
                // Numeric type makes it hard to give custom error messages
                type="text"
                value={n}
                onChange={this.handleNChanger} 
            />
            {error.length > 0 && <div>{error}</div>}
            {error.length == 0 && <div>We have {permutations.length} permutations.</div>}
            {this.state.loadingPermutations && <CircularProgress style={{color: "black"}}/>}
            <div className="flexrow block50">
                {permutations.map(perm => <div className="flexcol box">{perm.map(row => <div>{row.join(' ')}</div>)}</div>)}
            </div>
        </div>
        );
    }
}