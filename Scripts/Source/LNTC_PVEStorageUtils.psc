Scriptname LNTC_PVEStorageUtils

; Sets an integer into the JDB Database.
Function SetInt(String _namespace, String _key, Int _value) Global
    JDB.solveIntSetter("." + _namespace + "." + _key, _value, true)
EndFunction

; Sets a string into the JDB Database.
Function SetString(String _namespace, String _key, String _value) Global
    JDB.solveStrSetter("." + _namespace + "." + _key, _value, true)
EndFunction

; Sets a JMap into the JDB Database.
Function SetJMap(String _namespace, String _key, Int _mapIdentifer) Global
    JDB.solveObjSetter("." + _namespace + "." + _key, _mapIdentifer, true)
EndFunction

; Gets an int from the JDB Database.
Int Function GetInt(String _namespace, String _key, Int _defaultValue = 0) Global
    Return JDB.solveInt("." + _namespace + "." + _key, _defaultValue)
EndFunction

; Gets a string from the JDB Database.
String Function GetString(String _namespace, String _key, String _defaultValue = "") Global
    Return JDB.solveStr("." + _namespace + "." + _key, _defaultValue)
EndFunction

; Gets a JContainer (JMap/JArray) from the JDB Database.
Int Function GetJContainer(String _namespace, String _key, Int _defaultContainer = 0) Global
    Return JDB.solveObj("." + _namespace + "." + _key, _defaultContainer)
EndFunction

; Gets a JArray, adds the given String to the end of it (if it does not exist), and sets it back.
Function AddStringToJArray(String _namespace, String _key, String _value) Global
    Int i = GetJContainer(_namespace, _key, JArray.object())
    If JArray.findStr(i, _value) != -1
        Return
    EndIf
    JArray.addStr(i, _value)
    JDB.solveObjSetter("." + _namespace + "." + _key, i, true)
EndFunction

; Gets a JArray, removes all occurences of the given String from it, and sets it back
Function RemoveStringFromJArray(String _namespace, String _key, String _value) Global
    Int i = GetJContainer(_namespace, _key, JArray.object())
    If JArray.findStr(i, _value) == -1
        Return
    EndIf
    JArray.eraseString(i, _value)
    JDB.solveObjSetter("." + _namespace + "." + _key, i, true)
EndFunction