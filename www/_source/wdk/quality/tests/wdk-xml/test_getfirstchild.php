<?php
	
	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CXMLElement::GetFirstChild*ByName");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			//$this->SetResult(true);	
			return true;
		}
		
	
		
		function CallbackTest()
		{
			parent::CallbackTest();
	
	
$strXML="
<TEST>
	<TAG>Some Data 1</TAG>
	<TAG>Some Data 2</TAG>
</TEST>
";

			$this->Trace("Reference XML:");
			$this->Trace($strXML);
		
			$xml = new CXMLElement();
			$bResult = $xml->Parse($strXML);
			if ($bResult != true)
			{
				$this->Trace("Parsing failed. Test failed.");
				return;	
			}
			$this->Trace("xml:");
			$this->Trace($xml->GetRecursiveArray());			
						
			
			$xmlFirstChild = $xml->GetFirstChildByName("TAG");
			$this->Trace("xmlFirstChild:");
			$this->Trace($xmlFirstChild->GetRecursiveArray());			
			if ($xmlFirstChild == false)
			{
				$this->Trace("GetFirstChildByName(\"TAG\") returned false. Test failed!");	
				return;
			}
			$strName = $xmlFirstChild->GetName();
			if ($strName != "TAG")
			{
				$this->Trace("xmlFirstChild->GetName() returned \"$strName\". Test failed!");	
				return;
			}
			$strData = $xmlFirstChild->GetData();
			if ($strData != "Some Data 1")
			{
				$this->Trace("xmlFirstChild->GetData() returned \"$strData\". Test failed!");	
				return;
			}




			
			$strFirstChildData = $xml->GetFirstChildDataByName("TAG");
			$this->Trace("GetFirstChildDataByName(\"TAG\") returns \"$strFirstChildData\"");
			if ($strFirstChildData != "Some Data 1")
			{
				$this->Trace("Unexpected data. Test failed!");
				return;
			}
			
			$this->SetResult(true);
		}
		

		
	}
	
	

		
