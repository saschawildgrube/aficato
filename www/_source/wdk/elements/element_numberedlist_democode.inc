$this->IncludeElement("numberedlist");

$numberedlist = new CNumberedListElement($this->GetWebsite(),"default");
$arrayList = array();
for ($nIndex = 0; $nIndex < 5; $nIndex++)
{
	$arrayList[$nIndex] = "Item Number $nIndex";
}
$strOutput = $numberedlist->RenderNumberedList($arrayList, 10);