$arrayMessages = array();
$arrayMessages["info"] = array();
$arrayMessages["info"][] = "This is one info message.";
$arrayMessages["warning"] = array();
$arrayMessages["warning"][] = "This is a warning message.";
$arrayMessages["warning"][] = "This is another warning message.";
$arrayMessages["error"] = array();
$arrayMessages["error"][] = "This is an error message.";
$arrayMessages["error"][] = "This is another error message.";



$messagestack = new CMessageStackElement($this->GetWebsite(),"default");
$strOutput = $messagestack->RenderMessageStack(
	$arrayMessages);