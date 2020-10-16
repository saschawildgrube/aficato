$this->IncludeElement('countdown');

$countdown = new CCountDownElement($this->GetWebsite(),'default');
$strOutput = $countdown->RenderCountDown(GetTimeAddDays(GetTimeNow(),1));