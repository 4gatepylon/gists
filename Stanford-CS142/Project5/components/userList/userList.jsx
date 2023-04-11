import React from 'react';
import { Link } from 'react-router-dom'
import {
  Divider,
  List,
  ListItem,
  ListItemButton,
  ListItemText,
  Typography,
}
from '@mui/material';
import './userList.css';
import { cs142models } from '../../modelData/photoApp';
import PhotoAppContext from '../../modelData/PhotoAppContext'

/**
 * Define UserList, a React componment of CS142 project #5
 */
class UserList extends React.Component {
  static contextType = PhotoAppContext

  constructor(props) {
    super(props);
    this.state = {
      users: cs142models.userListModel()
    }
  }

  render() {
    const { currentUser, setCurrentUser } = this.context

    // TODO figure out a way to unify links to avoid these two onclicks
    // (alternatively...)
    return (
      <div>
        <Typography variant="h6">
          Users
        </Typography>
        <List component="nav">
          {this.state.users.map((user, idx) => 
            <div key={idx}>
              <Link to={`/users/${user}`}>
                <ListItemButton onClick={() => setCurrentUser(user)}>
                  <ListItemText disableTypography primary={
                    (() => {
                      const match = currentUser && user._id === currentUser._id;
                      return (
                        <Typography variant="body2" style={{ fontWeight: match ? 'bold' : '' }}>
                          {`${user.first_name} ${user.last_name}`}
                        </Typography>
                      )
                    })()
                  } />
                </ListItemButton>
              </Link>
              <Divider />
            </div>
          )}
        </List>
      </div>
    );
  }
}

export default UserList;
