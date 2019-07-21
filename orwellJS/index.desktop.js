/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Button,
  Text,
  View,
  FlatList,
  TouchableOpacity
} from 'react-native';
import NiceButton from './NiceButton';
const NativeModules = require('react-native').NativeModules;
const requireNativeComponent = require('requireNativeComponent');
var OrwellMediaPlayer = requireNativeComponent('OrwellMediaPlayer');
var GlueManager = require('react-native').NativeModules.GlueManager;

GlueManager.addRTSPStream("cam1", "rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam2", "rtsp://admin:19929394@192.168.0.102:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam3", "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam4", "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_1");

const MAIN_CAMERAS = 0;
const CONFIGURATIONS = 1;


class Orwell extends Component {
  constructor() {
    super();
  }

  //High order function to make onPress=... cleaner :)
  goToPage = (page) => () => this.setState({currentView:  page});

  state = {currentView: 0};

  render() {
    switch (this.state.currentView) {
      case MAIN_CAMERAS:
        return (
          <View style={{backgroundColor: '#303030'}}>
            <FlatList
              data={[{id: "cam1"}, {id: "cam2"}, {id: "cam3"}, {id: "cam4"}]} 
              renderItem={({ item }) => (
                  <OrwellMediaPlayer id={item.id} width={640} height={360}/>
              )}
              numColumns={2}
              keyExtractor={(item, index) => index}
            />
            <NiceButton
              label = "Configurations"
              buttonStyle={{position: 'absolute', right: 0, bottom: 0}}
              onPress={this.goToPage(CONFIGURATIONS)}
            />
          </View>
        )
        case CONFIGURATIONS:
        return(
        <View style={{backgroundColor: '#303030', width:1280, height:720}}>
          <Text style={{color: '#FFFFFF'}}>Another page 2</Text>
          <NiceButton
            label = "Cameras"
            buttonStyle={{position: 'absolute', right: 0, bottom: 0}}
            onPress={this.goToPage(MAIN_CAMERAS)}
            />
        </View>
      )
    }
  }
}

const styles = StyleSheet.create({
  ButtonOpacity:{
    position: 'absolute',
    right: 0,
    bottom: 0,
    marginRight:40,
    marginBottom:40,
    marginLeft:40,
    marginTop:10,
    paddingTop:10,
    paddingBottom:10,
    backgroundColor:'#000000',
  },
  ButtonText:{
      color:'#fff',
      textAlign:'center',
      paddingLeft : 10,
      paddingRight : 10
  },
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

