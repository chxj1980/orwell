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
  View,
  FlatList,
  TouchableOpacity
} from 'react-native';
import NiceButton from './NiceButton';
import CamerasViewer from './CamerasViewer';
import SettingsViewer from './SettingsViewer';

const NativeModules = require('react-native').NativeModules;
const requireNativeComponent = require('requireNativeComponent');
var GlueManager = require('react-native').NativeModules.GlueManager;

GlueManager.addRTSPStream("cam1", "rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam2", "rtsp://admin:19929394@192.168.0.102:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam3", "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam4", "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_1");

const VIEWS = {
  MAIN_CAMERAS: 0,
  CONFIGURATIONS: 1,
}
var cameras = [{id: "cam1"}, {id: "cam2"}, {id: "cam3"}, {id: "cam4"}];

class Orwell extends Component {
  constructor() {
    super();
  }
  state = {currentView: VIEWS.MAIN_CAMERAS};

  //High order function to make onPress=... cleaner :)
  goToPage = (page) => () => this.setState({currentView:  page});

  globalObjects = {
    VIEWS: VIEWS,
    changePageFunction: this.goToPage,
    rootStyle: styles.rootStyle,
  }

  render() {
    switch (this.state.currentView) {
      case VIEWS.MAIN_CAMERAS:
        return (
          <CamerasViewer cameras={cameras} globalObjects={this.globalObjects}/>
        )
      case VIEWS.CONFIGURATIONS:
        return(
          <SettingsViewer globalObjects={this.globalObjects}/>
        )
      default: 
        return(
          <View style={this.globalObjects.rootStyle}><Text style={{color: '#FFFFFF'}}>UNDEFINED PAGE</Text></View>
        )
    }
  }
}

const styles = StyleSheet.create({
  rootStyle: {
    backgroundColor: '#303030', 
    flex:1
  },
});

AppRegistry.registerComponent('Orwell', () => Orwell);