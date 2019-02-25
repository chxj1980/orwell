/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

const NativeModules = require('NativeModules');
const requireNativeComponent = require('requireNativeComponent');
var OrwellMediaPlayer = requireNativeComponent('RCTMediaPlayer');


class Orwell extends Component {
  render() {
    return (
      <View style={styles.container}>
        <OrwellMediaPlayer uri="rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0"/>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#242633',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#FFFFFF',
    marginBottom: 5,
  },
});

AppRegistry.registerComponent('Orwell', () => Orwell);

