<?php

	require_once(GetWDKDir()."wdk_securedatacontainer.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CSecureDataContainer");
		}
		
		function CallbackInit()
		{
			if (!IsMcryptSupported())
			{
				$this->SetActive(false);
			}
			return parent::CallbackInit();	
		}


		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			
			$dc = new CSecureDataContainer();
			
			
			$dc->SetValue("value1","key1");
			$dc->SetValue("value21","key2","key21");
			$this->Trace($dc->GetDataArray());
			
			$strSerialized = $dc->SerializeEncrypt("123");
			
			$this->Trace("strSerialized = \"$strSerialized\"");
			
			$dc2 = new CSecureDataContainer();
			
			$dc2->UnserializeDecrypt($strSerialized,"123");
			$this->Trace($dc2->GetHeaderArray());
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
	
	

		
