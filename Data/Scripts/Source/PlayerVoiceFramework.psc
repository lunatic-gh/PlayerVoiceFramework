Scriptname PlayerVoiceFramework Hidden
{ Some Helpful Papyrus Functions relayed from the plugin. }

; Returns whether or not debug mode is enabled.
Bool Function IsDebugMode() Global Native

; Enables or disables debug mode.
Function SetDebugMode(Bool _value) Global Native

; Logs an info message
Function LogInfo(String _message) Global Native

; Logs a warning message
Function LogWarn(String _message) Global Native

; Logs an error message
Function LogError(String _message) Global Native

; Logs a critical error message
Function LogCritical(String _message) Global Native

; Logs a debug message
Function LogDebug(String _message) Global Native

; Returns whether or not the given form has the given keyword.
Bool Function FormHasKeyword(Form _form, String _keyword) Global Native

; Returns the form from the given plugin-name and formID, or null if none was found.
Form Function FormFromID(String _pluginName, Int _formId) Global Native

; Returns the given form in the string-format "Plugin|FormID", or an empty string if the form was null
String Function FormToString(Form _form) Global Native

; Returns the form from the given string-format, or null if found. See above.
Form Function FormFromString(String _formString) Global Native

; Returns the given string with all occurences of _oldSeq replaced with _newSeq.
String Function ReplaceInString(String _text, String _oldSeq, String _newSeq) Global Native

; Returns the given string with all leading & trailing whitespaces removed
String Function TrimString(String _text) Global Native

; Sends a sound-event with the given name.
Function SendSoundEvent(String _name) Global Native

; Returns a random int between _minInclusive and _maxInclusive
Int Function RandomInt(Int _minInclusive, Int _maxInclusive) Global Native

; Returns a random float between _minInclusive and _maxInclusive
Float Function RandomFloat(Float _minInclusive, Float _maxInclusive) Global Native

; Returns the name of the currently active sound-pack, or an empty string if no pack is active.
String Function GetActivePack() Global Native

; Sets the pack with the given name as active. Does nothing if no pack with the given name was found.
Function SetActivePack(String _name) Global Native

; Returns a string-array with the names of all packs found in the user's load-order, no matter if inactive or not.
String[] Function GetPacks() Global Native