$this->IncludeElement("datetime");

$datetime = new CDateTimeElement($this->GetWebsite(), "default");
$strOutput = $datetime->RenderDateTime(GetTimeNow(),"Europe/Berlin");