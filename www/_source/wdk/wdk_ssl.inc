<?php

	function GetSSLCertificatesFromHost($strHost)
	{
		$arrayOptions = array();
		$arrayOptions['ssl']['capture_peer_cert'] = true;
		$arrayOptions['ssl']['capture_peer_cert_chain'] = true;
		$streamcontext = stream_context_create($arrayOptions);
		$ea = new CErrorAmnesty();
		$ea->Start();
		$streamsocket = stream_socket_client('ssl://'.$strHost.':443', $errno, $errstr, 30, STREAM_CLIENT_CONNECT, $streamcontext);
		$ea->End();
		if ($streamsocket == false)
		{
			return false;	
		}
		$arrayParams = stream_context_get_params($streamsocket);
		if (!is_array($arrayParams))
		{
			return false;	
		}
		$arrayResult = array();
		$arrayCertificateChain = $arrayParams['options']['ssl']['peer_certificate_chain'];
		if (!is_array($arrayCertificateChain))
		{
			return false;
		}
		foreach ($arrayCertificateChain as $arrayCert)
		{
			$arrayResult[] = openssl_x509_parse($arrayCert);
		}
		return $arrayResult;
	}
	

 	

