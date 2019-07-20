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

export default class NiceButton extends Component {  
  constructor() {
    super();
  }
  state = { currentView: 0 };

  render() {
      return (

          <TouchableOpacity
                  style={styles.ButtonOpacity}
                  onPress={()=>this.setState({currentView:1})}
                  underlayColor='#fff'>
                  <Text style={styles.ButtonText}>
                    Configurations
                  </Text>
          </TouchableOpacity>        
      )
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
});
