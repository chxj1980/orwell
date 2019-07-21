import {
    StyleSheet,
} from 'react-native'

export default function mergeObjects(obj1, obj2) {
    obj1_ = obj1 ? StyleSheet.flatten(obj1) : {};
    obj2_ = obj2 ? StyleSheet.flatten(obj2) : {};

    return { ...obj1_, ...obj2_ };
}