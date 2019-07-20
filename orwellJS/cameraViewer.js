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

export default class CameraViewer extends Component {  
  constructor() {
    super();
  }
  state = { currentView: 0 };

  render() {
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
          <TouchableOpacity
                  style={styles.ButtonOpacity}
                  onPress={()=>this.setState({currentView:1})}
                  underlayColor='#fff'>
                  <Text style={styles.ButtonText}>
                    Configurations
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

