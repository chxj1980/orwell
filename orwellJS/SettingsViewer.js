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

export default function SettingsViewer({
  globalObjects={},
})
{
  return (
    <View style={globalObjects.rootStyle}>
      <Text style={{color: '#FFFFFF'}}>Configurations</Text>
      <NiceButton
        label = "Cameras"
        buttonStyle={{position: 'absolute', right: 0, bottom: 0}}
        onPress={globalObjects.changePageFunction(globalObjects.VIEWS.MAIN_CAMERAS)}
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

