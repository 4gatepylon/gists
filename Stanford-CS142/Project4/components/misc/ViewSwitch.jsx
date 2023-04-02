import React from 'react'
import { HashRouter, Route, Link, Switch } from "react-router-dom";
import './ViewSwitch.css'

export class ViewSwitch extends React.Component {
    constructor(props) {
        super(props)
        this.options = props.options;
        this.defaultAction = Object.keys(this.options)[0]
        this.state = {
            tentativeOption: this.defaultAction,
            curentOption: this.defaultAction,
        }

        this.handleTentativeOptionChange = this.handleTentativeOptionChange.bind(this)
        this.handleCurrentOptionChange = this.handleCurrentOptionChange.bind(this)
    }

    handleTentativeOptionChange(e) {
        this.setState({tentativeOption: e.target.value})
    }

    handleCurrentOptionChange() {
        const newOption = this.state.tentativeOption;
        this.setState({tentativeOption: this.defaultAction, currentOption: newOption})
    }

    render () {
        return <div>
            <select>
                {Object.keys(this.options)
                    .map(keyName => <option value={keyName} onClick={this.handleTentativeOptionChange}>{keyName}</option>)}
            </select>
            <button onClick={this.handleCurrentOptionChange}>Display {this.state.tentativeOption}</button>
            <div className='blocker20px'/>
            {Object.keys(this.options).map((keyName) => keyName == this.state.currentOption && this.options[keyName])}
        </div>
    }

}

class Test extends React.Component {
    constructor(props) { super(props) }
    render() {
        return <div>{keyName}</div>
    }
}

export class ViewSwitchWithRouter extends React.Component {
    constructor(props) {
        super(props)
        this.options = props.options;
    }

    render() {
        return <>
            <HashRouter>
                <div className="flexrow">
                    {Object.keys(this.options).map(keyName => <Link className="link" to={`/${keyName}`}>Go to "{keyName}"</Link>)}
                </div>
                <Switch>
                    {Object.keys(this.options).map((keyName) => {
                        return <Route path={`/${keyName}`} component={this.options[keyName]} />
                    })}
                </Switch>
            </HashRouter>
        </>
    }
}