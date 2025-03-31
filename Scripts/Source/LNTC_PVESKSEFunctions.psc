Scriptname LNTC_PVESKSEFunctions Hidden
; This files includes a few helper-functions from the SKSE Plugin.

Function PlaySound(String _category, String _subCategory = "") Global Native

Function StopSound() Global Native

Function Log(String _msg) Global Native

String Function Replace(String _text, String _old, String _new) Global Native

Function RegisterFloatCondition(String _name, Float _value) Global Native

Function RegisterIntCondition(String _name, Int _value) Global Native

Function RegisterBoolCondition(String _name, Bool _value) Global Native

Function RegisterStringCondition(String _name, String _value) Global Native