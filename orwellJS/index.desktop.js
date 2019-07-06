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
  FlatList
} from 'react-native';

const NativeModules = require('react-native').NativeModules;
const requireNativeComponent = require('requireNativeComponent');
var OrwellMediaPlayer = requireNativeComponent('RCTMediaPlayer');

class Orwell extends Component {
  componentDidMount() {
    
  }
  render() {
    return (
      <View style={{flex: 1, flexDirection: 'row'}}>
        <OrwellMediaPlayer uri='rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0' width={640} height={360}/>
        <OrwellMediaPlayer uri='rtsp://admin:19929394@192.168.1.72:10554/tcp/av0_0' width={640} height={360}/>
      </View>
    )
    
  }
}
//<OrwellMediaPlayer uri='rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0' width={640} height={360}/>
/*
<FlatList data={[{uri: 'rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0'}]}
                renderItem={({item}) => <OrwellMediaPlayer uri={item.uri} width={640} height={360}/>}>
      </FlatList>
*/
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

