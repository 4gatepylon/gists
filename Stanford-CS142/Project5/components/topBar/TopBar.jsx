import React from 'react';
import {
  AppBar, Toolbar, Typography
} from '@mui/material';
import './TopBar.css';
import { cs142models } from '../../modelData/photoApp';
import '../style/General.css'

/**
 * Define TopBar, a React componment of CS142 project #5
 */
class TopBar extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      developerName: cs142models.developerName()
    }
  }

  render() {
    return (
      <AppBar className="cs142-topbar-appBar" position="absolute">
        <Toolbar className="cs142-space-between">
          <div className="cs142-left">
            <Typography variant="h5" color="inherit">
                Developed by {this.state.developerName}
            </Typography>
          </div>
          <Typography>
            Insert Context Here
          </Typography>
        </Toolbar>
      </AppBar>
    );
  }
}

export default TopBar;
