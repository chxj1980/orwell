/**
 * Simple Nice Button with styles that can be merged
 * Lucas Zanella
 */

import React from 'react';
//console.log(React);
import {
  StyleSheet,
  Text,
  TouchableOpacity,
} from 'react-native';
import mergeObjects from './mergeObjects'

export default function NiceButton({
  onPress = ()=>null,
  label = 'Click me',
  buttonStyle = {},
  textColor = {},
}) {
  console.log(mergeObjects(buttonStyle,styles.TouchableOpacityStyle));

  return (
    <TouchableOpacity style={mergeObjects(buttonStyle,styles.TouchableOpacity)} onPress={onPress}>
        <Text style={mergeObjects(textColor,styles.ButtonText)}>{label}</Text>
    </TouchableOpacity>
  );
}

//TODO: why there's no PropTypes on React?
NiceButton.propTypes = {
  /*
  onPress: React.PropTypes.func.isRequired,
  label: React.PropTypes.string,
  buttonStyle: React.PropTypes.any,
  textColor: React.PropTypes.any,
  */
};

const styles = StyleSheet.create({
  TouchableOpacity:{
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
