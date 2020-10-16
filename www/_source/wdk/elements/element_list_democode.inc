$list = new CListElement($this->GetWebsite(),"default");
$arrayList = array();
for ($nIndex = 0; $nIndex < 5; $nIndex++)
{
	$arrayList[$nIndex] = $nIndex;
}
$strOutput = $list->RenderList($arrayList);