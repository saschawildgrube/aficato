<?php

	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ParseHttpResponseEx");
		}
		

		function TestCase_ParseHttpResponseEx(
			$strResponse,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ParseHttpResponseEx");
	
			if (is_array($arrayExpectedResult))
			{
				$this->Trace("Expected Result:");
				$this->Trace($arrayExpectedResult);
			}
			else
			{
				$this->Trace("Expected Result: ".RenderBool($arrayExpectedResult));
			}

			$arrayResult = ParseHttpResponseEx($strResponse);
			
			if (is_array($arrayResult))
			{
				$this->Trace("Result:");
				$this->Trace($arrayResult);
			}
			else
			{
				$this->Trace("Result: ".RenderBool($arrayResult));
			}
				
			if ($arrayResult != $arrayExpectedResult)
			{
				$this->Trace("Testcase FAILED!");	
				$this->Trace("");
				$this->Trace("");
				$this->SetResult(false);	
				return;
			}
	
			$this->Trace("Testcase PASSED!");
			$this->Trace("");
			$this->Trace("");
			
		}



		function OnTest()
		{
			parent::OnTest();
			
			$this->SetResult(true);
			
			$strResponse = 'HTTP/1.1 200 OK
Date: Fri, 08 Nov 2013 22:53:16 GMT
Server: Apache
Set-Cookie: sessionid=f7cf4cdc884273dc0633; expires=Sun, 10-Nov-2013 22:53:17 GMT
Set-Cookie: a=b=1:c=2; expires=Sun, 10-Nov-2013 22:53:17 GMT
Location: http://www.example.com/redirect
Content-Type: text/html

<!DOCTYPE html>
<html><body>
<a href="http://www.example.com">www.example.com</a><br/>
<form role="form" action="http://www.example.com/" method="post" enctype="multipart/form-data">
<input type="text" id="TEXT1" name="text1" value="This is some text" size="35" maxlength="500"/>
<select id="SELECT1" name="select1">
<option value="option1">ONE</option>
<option value="option2">TWO</option>
</select>
<textarea name="textarea1" id="TEXTAREA1" rows="6" cols="10">Some more text
with multiple lines</textarea>
<input type="hidden" name="hidden1" value="hello"/>
<input type="hidden" name="hidden2" value="world"/>
</form>
<a href="http://www.example.com/about/">About/</a><br/>
</body></html>';

			//$this->Trace("1:".RenderHex($strResponse));

			$arrayExpectedResponse = array(
				"headers" =>
'HTTP/1.1 200 OK
Date: Fri, 08 Nov 2013 22:53:16 GMT
Server: Apache
Set-Cookie: sessionid=f7cf4cdc884273dc0633; expires=Sun, 10-Nov-2013 22:53:17 GMT
Set-Cookie: a=b=1:c=2; expires=Sun, 10-Nov-2013 22:53:17 GMT
Location: http://www.example.com/redirect
Content-Type: text/html',
				"content" =>
'<!DOCTYPE html>
<html><body>
<a href="http://www.example.com">www.example.com</a><br/>
<form role="form" action="http://www.example.com/" method="post" enctype="multipart/form-data">
<input type="text" id="TEXT1" name="text1" value="This is some text" size="35" maxlength="500"/>
<select id="SELECT1" name="select1">
<option value="option1">ONE</option>
<option value="option2">TWO</option>
</select>
<textarea name="textarea1" id="TEXTAREA1" rows="6" cols="10">Some more text
with multiple lines</textarea>
<input type="hidden" name="hidden1" value="hello"/>
<input type="hidden" name="hidden2" value="world"/>
</form>
<a href="http://www.example.com/about/">About/</a><br/>
</body></html>',
				"statuscode" => "200",
				"redirect-location" => "http://www.example.com/redirect",
				"cookies" => array(
					"sessionid" => "f7cf4cdc884273dc0633",
					"a" => "b=1:c=2" 
					),
				"links" => array(
					"http://www.example.com",
					"http://www.example.com/about/"),
				"forms" => array(
					0 => array(
						"action" => "http://www.example.com/",
						"method" => "post",
						"values" => array(
							"text1" => "This is some text",
							"hidden1" => "hello",
							"hidden2" => "world",
							"textarea1" => "Some more text
with multiple lines",
							"select1" => ""
							)
						)
	        )
				);
			$this->TestCase_ParseHttpResponseEx($strResponse,$arrayExpectedResponse);


		}
	}
	
	

		
