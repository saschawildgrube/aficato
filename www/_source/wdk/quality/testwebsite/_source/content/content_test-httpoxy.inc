<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$arrayItems["_SERVER_HTTP_PROXY"] = ArrayGetValue($_SERVER,"HTTP_PROXY");
			$arrayItems["_GETENV_HTTP_PROXY"] = getenv("HTTP_PROXY");
			
		}
	}
	
