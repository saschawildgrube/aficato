$this->IncludeElement("mailicon");

$mailicon = new CMailIconElement($this->GetWebsite(),"default");
$strOutput = $mailicon->RenderMailIcon(
	"info@example.com",
	"Send a mail to info@example.com");