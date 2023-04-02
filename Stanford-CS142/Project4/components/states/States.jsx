import React from 'react';
import './States.css';

/**
 * Define States, a React component of CS142 Project 4, Problem 2. The model
 * data for this view (the state names) is available at
 * window.cs142models.statesModel().
 */
class States extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      states: window.cs142models.statesModel(),
      substring: '',
      matchingStates: [],
    }
    this.handleSubstringChange= this.handleSubstringChange.bind(this)
  }

  handleSubstringChange (e) {
    let newSubstr = e.target.value;
    let newMatchingStates = this.state.states.filter((state) => state.toLowerCase().includes(newSubstr.toLowerCase()));
    newMatchingStates.sort();
    this.setState({ matchingStates: newMatchingStates, substring: newSubstr });
  }

  render() {
    const { substring, matchingStates } = this.state;
    return (
      <div>
        <div>
          <label htmlFor="substringChanger">Add to your substring: </label>
          <input 
            id="substringChanger"
            type="text"
            value={substring}
            onChange={this.handleSubstringChange} 
          />
          <p>You entered: {`"${substring}"`}</p>
        </div>
        <div>
          {matchingStates.length == 0 && substring.length > 0 && <p>No states found</p>}
          {matchingStates.length == 0 && substring.length == 0 && <p>Enter a substring</p>}
          {matchingStates.length > 0 && <ul>{matchingStates.map((state) => <li className="hoverMe">{state}</li>)}</ul>}
        </div>
      </div>
    );
  }
}

export default States;
