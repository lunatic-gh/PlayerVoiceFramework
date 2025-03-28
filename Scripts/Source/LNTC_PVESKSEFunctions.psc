Scriptname LNTC_PVESKSEFunctions Hidden
; This files includes a few helper-functions from the SKSE Plugin.

Function PlaySound(String _category, String _subCategory = "") Global Native

Function StopSound() Global Native

Function Log(String _msg) Global Native

String Function Replace(String _text, String _old, String _new) Global Native

Function RegisterCondition(String _name, String _value) Global Native