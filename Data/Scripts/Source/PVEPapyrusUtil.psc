Scriptname PVEPapyrusUtil Hidden
{ Some Helpful Papyrus Functions relayed from the plugin. }

Form Function FormFromID(String _pluginName, Int _formId) Global Native

String Function FormToString(Form _form) Global Native

Form Function FormFromString(String _formString) Global Native

String Function ReplaceInString(String _text, String _oldSeq, String _newSeq) Global Native

String Function TrimString(String _text) Global Native

Bool Function PlaySoundEvent(String _name) Global Native

Int Function RandomInt(Int _minInclusive, Int _maxInclusive) Global Native

Float Function RandomFloat(Float _minInclusive, Float _maxInclusive) Global Native