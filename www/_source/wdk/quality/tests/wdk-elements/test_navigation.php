<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Navigation");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			$this->SetResult(true);			
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$strURL_Root = "http://".GetRootURL()."quality/testwebsite/";
			$strURL = $strURL_Root."?content=test-element-navigation";
			
			$this->TestCase_CheckURL(
				$strURL,
				array(
'<nav>
<a href="'.$strURL_Root.'en/">Start</a>
<br/>
&nbsp;<a href="'.$strURL_Root.'en/test1/">Test 1</a>
<br/>
&nbsp;&nbsp;<a href="'.$strURL_Root.'en/test11/">Test 1.1</a>
<br/>
&nbsp;&nbsp;<a href="'.$strURL_Root.'en/test12/">Test 1.2</a>
<br/>
&nbsp;<a href="'.$strURL_Root.'en/test2/">Test 2</a>
<br/>
&nbsp;&nbsp;<a href="'.$strURL_Root.'en/test21/">Test 2.1</a>
<br/>
&nbsp;&nbsp;<a href="'.$strURL_Root.'en/test22/">Test 2.2</a>
<br/>
&nbsp;<a href="'.$strURL_Root.'en/test3/">Test 3</a>
<br/>
&nbsp;&nbsp;<a href="'.$strURL_Root.'en/test31/">Test 3.1</a>
<br/>
&nbsp;&nbsp;<a href="'.$strURL_Root.'en/test-element-navigation/?param=test32">Test 3.2</a>
</nav>'
));
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
	}
		
