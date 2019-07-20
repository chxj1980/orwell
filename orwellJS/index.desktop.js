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

const NativeModules = require('react-native').NativeModules;
const requireNativeComponent = require('requireNativeComponent');
var OrwellMediaPlayer = requireNativeComponent('OrwellMediaPlayer');
var GlueManager = require('react-native').NativeModules.GlueManager;

GlueManager.addRTSPStream("cam1", "rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam2", "rtsp://admin:19929394@192.168.0.102:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam3", "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
GlueManager.addRTSPStream("cam4", "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_1");

goTo = function(state) {
  if (state==0) {
    return 1;
  } else if (state==1) {
    return 0;
  }
}

class Orwell extends Component {
  constructor() {
    super();
  }
  state = { currentView: 0 };

  render() {
    if (this.state.currentView==0) {
      return (
        <View>
          <FlatList
            data={[{id: "cam1"}, {id: "cam2"}, {id: "cam3"}, {id: "cam4"}]} 
            renderItem={({ item }) => (
                <OrwellMediaPlayer id={item.id} width={640} height={360}/>
            )}
            numColumns={2}
            keyExtractor={(item, index) => index}
          />
          <TouchableOpacity
                  width={100}
                  style={styles.ButtonOpacity}
                  onPress={()=>this.setState({currentView:1})}
                  underlayColor='#fff'>
                  <Text style={styles.ButtonText}>
                    Configurations
                  </Text>
          </TouchableOpacity>
        </View>
        
      )
    } else if (this.state.currentView==1) {
      return(
        <View>
          <Text >Another page</Text>
          <TouchableOpacity
                  width={100}
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
}
//<OrwellMediaPlayer uri='rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0' width={640} height={360}/>
/*
<FlatList data={[{uri: 'rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0'}]}
                renderItem={({item}) => <OrwellMediaPlayer uri={item.uri} width={640} height={360}/>}>
      </FlatList>
*/
/*
<FlatList data={[{id: "cam1"}, {id: "cam2"}]} 
                style={{flex: 1, flexDirection: 'row'}}
                renderItem={({item}) => <OrwellMediaPlayer id={item.id} width={640} height={360}/>}
                keyExtractor={(item, index) => index.toString()}
      />
*/
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

