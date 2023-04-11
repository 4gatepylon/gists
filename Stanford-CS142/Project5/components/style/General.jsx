import React from 'react'

export class BoldIf extends React.Component {
    constructor (props) { super(props) }
    render () {
        return (
            <div className={this.props.check() ? 'cs142-bold' : ''}>
                {this.props.children}
            </div>
        )
    }
}