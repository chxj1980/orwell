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

const requireNativeComponent = require('requireNativeComponent');
var OrwellMediaPlayer = requireNativeComponent('OrwellMediaPlayer');
import NiceButton from './NiceButton';

export default function CamerasViewer({
  cameras = {},
  globalObjects={},
})
{
  return (
    <View style={globalObjects.rootStyle}>
      <FlatList
        data={cameras} 
        renderItem={({ item }) => (
            <OrwellMediaPlayer id={item.id} width={640} height={360}/>
        )}
        numColumns={2}
        keyExtractor={(item, index) => index}
      />
      <NiceButton
        label = "Configurations"
        buttonStyle={{position: 'absolute', right: 0, bottom: 0}}
        onPress={globalObjects.changePageFunction(globalObjects.VIEWS.CONFIGURATIONS)}
      />
    </View>
  )
}


const styles = StyleSheet.create({
  /*
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
  */
});

