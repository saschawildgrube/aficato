<?php

	interface IEventHandler
	{
		function EventHandler($strEventID, $arrayParams);
	}


	class CEventDispatcher
	{
		private $m_arrayEventHandler;
		
		function __construct()
		{
			$this->m_arrayEventHandler = array();
		}
		
		function RegisterEvent($strEventID, IEventHandler $eh)
		{
			//$this->UnregisterEvent($strEventID, $eh);
			$this->m_arrayEventHandler[$strEventID][] = $eh;
		}



		function TriggerEvent($strEventID, $arrayParams)
		{
			$arrayResults = array();
			$arrayHandlers = ArrayGetValue($this->m_arrayEventHandler,$strEventID);
			if (is_array($arrayHandlers))
			{
				foreach ($arrayHandlers as $handler)
				{
					if ($handler instanceof IEventHandler)
					{
						$arrayResults[] = $handler->EventHandler($strEventID,$arrayParams);	
					}
				}	
			}
			return $arrayResults;
		}
	}


