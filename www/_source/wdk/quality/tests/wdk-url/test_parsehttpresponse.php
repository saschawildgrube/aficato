<?php

	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ParseHttpResponse");
		}
		

		function TestCase_ParseHttpResponse(
			$strResponse,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ParseHttpResponse");
	
			if (is_array($arrayExpectedResult))
			{
				$this->Trace("Expected Result:");
				$this->Trace($arrayExpectedResult);
			}
			else
			{
				$this->Trace("Expected Result: ".RenderBool($arrayExpectedResult));
			}

			$arrayResult = ParseHttpResponse($strResponse);
			
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
			
			$strResponse = "HTTP/1.1 404 Not Found
Date: Fri, 08 Nov 2013 22:53:16 GMT
Server: Apache
Set-Cookie: sessionid=f7cf4cdc884273dc0633; expires=Sun, 10-Nov-2013 22:53:
17 GMT
Content-Type: text/html

<!DOCTYPE html>

<html><body></body></html>";

			//$this->Trace("1:".RenderHex($strResponse));

			$arrayResponse = array(
				"headers" => "HTTP/1.1 404 Not Found
Date: Fri, 08 Nov 2013 22:53:16 GMT
Server: Apache
Set-Cookie: sessionid=f7cf4cdc884273dc0633; expires=Sun, 10-Nov-2013 22:53:
17 GMT
Content-Type: text/html",
				"content" => "<!DOCTYPE html>

<html><body></body></html>",
				"statuscode" => "404"
				);
			//$this->Trace("2:".RenderHex($arrayResponse["headers"]));
			//$this->Trace("3:".RenderHex($arrayResponse["content"]));
			$this->TestCase_ParseHttpResponse($strResponse,$arrayResponse);


		}
	}
	
	

		
