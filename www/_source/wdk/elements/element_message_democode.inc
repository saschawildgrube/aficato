$message_info = new CMessageElement($this->GetWebsite(),"info");
$strOutput = $message_info->RenderMessage(
	"This is an information message.");

$message_warning = new CMessageElement($this->GetWebsite(),"warning");
$strOutput .= $message_warning->RenderMessage(
	"This is a warning message.");

$message_error = new CMessageElement($this->GetWebsite(),"error");
$strOutput .= $message_error->RenderMessage(
	"This is an error message.");