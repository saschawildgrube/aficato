<?php

	require_once(GetWDKDir()."wdk_datacontainer.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CDataContainer");
		}
		



		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			
			$dc = new CDataContainer();
			
			
			$dc->SetValue("value1","key1");
			$dc->SetValue("value21","key2","key21");
			$this->Trace($dc->GetDataArray());
			
			$strSerialized = $dc->Serialize();
			
			$this->Trace("strSerialized = \"$strSerialized\"");
			
			$dc2 = new CDataContainer();
			
			$dc2->Unserialize($strSerialized);
			$this->Trace($dc2->GetDataArray());
			
			if ($dc2->GetValue("key1") != "value1")
			{
				return;	
			}
			
			if ($dc2->GetValue("key2","key21") != "value21")
			{
				return;	
			}
			
					
			$this->SetResult(true);
			
		}
		
		
	}
	
	

		
