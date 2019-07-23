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
  Switch,
  Image
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

  SimpleTextInput = ({
    placeholder="",
    value="",
  }) => (
    <View style={styles.textInputBackground}>
      <TextInput style={styles.textInput} placeholder={placeholder} value={value}/>
    </View>
  );

  selectedCamera = "cam1";

  return (
    <View style={mergeObjects(globalObjects.rootStyle,{flexDirection: 'row'})}>
      <View style={styles.cameraSelector}> 
        <View style={{flex:1}}>
          <TouchableOpacity style={{flex:1, marginLeft: 10, marginTop: 10}} onPress={globalObjects.changePageFunction(globalObjects.VIEWS.MAIN_CAMERAS)}>
            <Image style={{width: 30, height: 30}} source={require('./icons/go-back-left-arrow.png')}/>
          </TouchableOpacity>
        </View>
        <View style = {{flex:13}}>
          <FlatList
            data={globalObjects.realm.objects('Camera')} 
            ItemSeparatorComponent={this.renderSeparator}
            renderItem={({ item }) => (
              <TouchableOpacity style={styles.touchOpcacity} onPress={()=>{selectedCamera=item.name;}}>
              <View style={{flexDirection: 'column'}}>
                <Text style={styles.cameraTitle}>{item.name}</Text>
                <Text style={styles.cameraSubTitle}>{item.host+":"+item.onvif}</Text>
              </View>
              </TouchableOpacity>
            )}
            keyExtractor = { (item, index) => item.name.toString() }
          />
        </View>
      </View>
      <View style={styles.settingsEditor}> 
        <View style={styles.cameraItem}><Text style={styles._cameraTitle}>Cam 1</Text></View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Host: </Text>
          <SimpleTextInput value="192.168.0.101"/>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Onvif Port: </Text>
          <SimpleTextInput value="10080"/>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>RTSP automatic: </Text>
          <Switch style={{flex:1}} onValueChange={console.log("changed")} value={true} color={"#BABABA"} trackColor={{false: "#BABABA", true: "#BABABA"}}/>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>RTSP Port: </Text>
          <SimpleTextInput placeholder="554"/>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Username</Text>
          <SimpleTextInput placeholder="admin"/>
        </View>
        <View style={styles.cameraItem}>
          <Text style={styles.settingsItem}>Password</Text>
          <SimpleTextInput placeholder="admin"/>
        </View>
      </View>
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
    color:"#a3a3a3",
    fontSize: 13,
  },
  cameraItem: {
    flexDirection:'row',
    marginBottom:10
  },
  _cameraTitle: {
    color:"#FFFFFF",
    fontSize: 22,
    flex:1,
    fontFamily: "Roboto"
  },
  settingsItem: {
    color:"#FFFFFF",
    fontSize: 22,
    flex:1,
    fontFamily: "Roboto"
  },
  textInput: {
    color:"#FFFFFF",
    fontSize: 22,
    flex:1,
    fontFamily: 'Roboto'
  },
  touchOpcacity: {
    flex:1,
    marginLeft: 7,
    marginBottom: 10,
  },
  textInputBackground: {
    flex:1,
    backgroundColor: '#59646f',
    marginRight: 13,
    marginLeft:13,
    borderRadius: 5,
  }
});

