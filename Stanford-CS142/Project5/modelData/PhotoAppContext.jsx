// We store all dynamic state inside a context because
// it is nice to seperate it from the static context is
// the models outside

import React from 'react'

const PhotoAppContext = React.createContext()

class PhotoAppContextProvider extends React.Component {
    constructor(props) {
      super(props)
      this.state =  {
          currentUser: null,
      }
    }

    render() {
      const { children } = this.props
      const { currentUser } = this.state
      const setCurrentUser = (user) => this.setState({ currentUser: user })
      
      return (
        <PhotoAppContext.Provider
          value={{
            currentUser,
            setCurrentUser,
          }}
        >
          {children}
        </PhotoAppContext.Provider>
      )
    }
  }
  
  export default PhotoAppContext
  
  export { PhotoAppContextProvider }