import React from 'react';
import ReactDOM from 'react-dom';

import States from './components/states/States';
import Header from './components/header/Header';
import Example from './components/example/Example'
import { ViewSwitch } from './components/misc/ViewSwitch'

ReactDOM.render(
  <>
    <Header />
    <ViewSwitch options={{
        "Example": <Example />,
        "States": <States />,
    }} />
  </>,
  document.getElementById('reactapp'),
);
