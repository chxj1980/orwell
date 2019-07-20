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

export default class SettingsViewer extends Component {  
  constructor() {
    super();
  }
  state = { currentView: 0 };

  render() {
      return(
        <View style={{backgroundColor: '#303030', width:1280, height:720}}>
          <Text style={{color: '#FFFFFF'}}>Another page 2</Text>
          <TouchableOpacity
                  style={styles.ButtonOpacity}
                  onPress={()=>this.setState({currentView:0})}
                  underlayColor='#fff'>
                  <Text style={styles.ButtonText}>
                    Cameras
                  </Text>
          </TouchableOpacity>
        </View>
      )
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
