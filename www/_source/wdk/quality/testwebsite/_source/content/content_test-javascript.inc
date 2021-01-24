<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->AddJavaScript("test1");
			$this->GetWebsite()->AddJavaScript("test2");
			$this->GetWebsite()->AddJavaScript("test3","test3.min");
			$this->GetWebsite()->AddJavaScriptFile("test4",GetWDKDir()."quality/testwebsite/_source/js/test4.js");
			$this->GetWebsite()->AddJavaScriptSourceCode("test5","// Java Script Test 5");
			$this->GetWebsite()->AddJavaScriptURL("test6","http://www.example.com/js/test6.js");
		}
	}
	
