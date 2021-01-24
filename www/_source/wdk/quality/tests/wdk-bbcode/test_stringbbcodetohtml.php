<?php
	
	require_once(GetWDKDir()."wdk_bbcode.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StrongBBCodeToHtml");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringBBCodeToHtml($strString,$strCSSClass,$strExpectedResult)
		{
			$this->Trace("TestCase_StringBBCodeToHtml");
			$this->Trace("strString      : \"$strString\"");
			$this->Trace("strCSSClass    : \"$strCSSClass\"");
			$this->Trace("Expected Result: \"$strExpectedResult\"");
			$strResult = StringBBCodeToHTML($strString,$strCSSClass);
			$this->Trace("StringBBCodeToHTML returns: \"$strResult\"");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}

		
		function OnTest()
		{
			parent::OnTest();
			
		
			$this->TestCase_StringBBCodeToHtml(
				"[b]Bold[/b][br]",
				"",
				"<strong>Bold</strong><br/>");

			$this->TestCase_StringBBCodeToHtml(
				"[b]Bold[/b][br]",
				"bbcode",
				"<strong class=\"bbcode\">Bold</strong><br class=\"bbcode\"/>");

			$this->TestCase_StringBBCodeToHtml(
				"[i][url=http://www.example.com]Example.com[/url][/i]",
				"",				
				"<em><a href=\"http://www.example.com\" target=\"_blank\">Example.com</a></em>");

			$this->TestCase_StringBBCodeToHtml(
				"[i][url=http://www.example.com]Example.com[/url][/i]",
				"bbcode",
				"<em class=\"bbcode\"><a href=\"http://www.example.com\" target=\"_blank\" class=\"bbcode\">Example.com</a></em>");

			$this->TestCase_StringBBCodeToHtml(
				"[quote]This is a citate.[/quote]",
				"",				
				"<blockquote>This is a citate.</blockquote>");

			$this->TestCase_StringBBCodeToHtml(
				"[quote=Source Reference]This is a citate.[/quote]",
				"",				
				"<blockquote>This is a citate.</blockquote><cite>Source Reference</cite>");

			$this->TestCase_StringBBCodeToHtml(
				"[video]http://www.youtube.com/watch?v=kNuW5yDaxTY&feature=popular[/video]",
				"",				
				"<embed src=\"http://www.youtube.com/v/kNuW5yDaxTY\" type=\"application/x-shockwave-flash\" width=\"425\" height=\"344\"></embed>");

			$this->TestCase_StringBBCodeToHtml(
				"[video]http://video.google.com/videoplay?docid=514425911425024420#[/video]",
				"",				
				"<embed src=\"http://video.google.com/googleplayer.swf?docid=514425911425024420\" width=\"400\" height=\"326\" type=\"application/x-shockwave-flash\"></embed>");


		}
		

	}
	
	

		
