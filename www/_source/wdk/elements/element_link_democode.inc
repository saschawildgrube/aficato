$list = new CListElement($this->GetWebsite(),'default');
$arrayList = array();
			
$link = new CLinkElement($this->GetWebsite(), 'default');
$strURL = 'http://www.example.com';
array_push($arrayList, $link->RenderLink($strURL, 'Example.com'));
		
$strURL = 'http://www.example.com';
array_push($arrayList, $link->RenderLink($strURL, 'Example.com (open in a new browser tab or window)', true));

$strOutput = $list->RenderList($arrayList);