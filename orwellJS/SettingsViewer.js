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
  TouchableOpacity,
  TextInput,
  Switch
} from 'react-native';

import NiceButton from './NiceButton';
import mergeObjects from './mergeObjects'

export default function SettingsViewer({
  globalObjects={},
})
{
  renderSeparator = () => (
    <View
      style={{
        backgroundColor: 'gray',
        height: 0.5,
      }}
    />
  );

  return (
    <View style={mergeObjects(globalObjects.rootStyle,{flexDirection: 'row'})}>
      <View style={styles.cameraSelector}> 
        <View style={{flex:1}}></View>
        <View style = {{flex:13}}>
          <FlatList
            data={globalObjects.realm.objects('Camera')} 
            ItemSeparatorComponent={this.renderSeparator}
            renderItem={({ item }) => (
              <TouchableOpacity style={styles.touchOpcacity} onPress={()=>null}>
              <View style={{flexDirection: 'column'}}>
                <Text style={styles.cameraTitle}>{item.name}</Text>
                <Text style={styles.cameraSubTitle}>{item.host}</Text>
              </View>
              </TouchableOpacity>
            )}
            keyExtractor = { (item, index) => index.toString() }
          />
        </View>
      </View>
      <View style={styles.settingsEditor}> 
        <View style={styles.cameraItem}><Text style={styles._cameraTitle}>Cam 1</Text></View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Host: </Text>
          <Text style={styles.settingsItem}>192.168.0.101</Text>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Onvif Port: </Text>
          <Text style={styles.settingsItem}>10080</Text>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>RTSP automatic: </Text>
          <Switch onValueChange = {console.log("changed")}/>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>RTSP Port: </Text>
          <Text style={mergeObjects(styles.settingsItem,{})}>10554</Text>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Username</Text>
          <Text style={styles.settingsItem}>admin</Text>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Password</Text>
          <Text style={styles.settingsItem}>19929394</Text>
        </View>
      </View>
     
      <NiceButton
        label = "Cameras"
        buttonStyle={{position: 'absolute', right: 0, bottom: 0}}
        onPress={globalObjects.changePageFunction(globalObjects.VIEWS.MAIN_CAMERAS)}
        />
    </View>
  )
}


const styles = StyleSheet.create({
  cameraSelector: {
    flex:8,
  },
  settingsEditor: {
    flex:20, 
    flexDirection:'column',
    marginTop: 10,
    marginLeft: 10,
  },
  cameraTitle: {
    flex:1,
    color:"#FFFFFF",
    fontSize: 20,
    fontWeight: 'bold'
  },
  cameraSubTitle: {
    color:"#FFFFFF",
    fontSize: 13,
  },
  cameraItem: {
    flexDirection:'row',
    marginBottom:10
  },
  _cameraTitle: {
    color:"#FFFFFF",
    fontSize: 22,
    flex:1
  },
  settingsItem: {
    color:"#FFFFFF",
    fontSize: 22,
    flex:1
  },
  touchOpcacity: {
    flex:1,
    marginLeft: 7,
    marginBottom: 10,
  }
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

