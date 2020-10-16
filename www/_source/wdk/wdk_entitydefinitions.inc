<?php

	require_once("wdk_mysql.inc");

	define("ENTITYDEF_CORE",0);
	define("ENTITYDEF_MODULE",1);
	define("ENTITYDEF_WEBSERVICE",2);
	
	define("ENTITY_FLOATPRECISION_DEFAULT",2);
	//define("ENTITY_STRINGLENGTH_MAX",65535);
	//define("ENTITY_STRINGLENGTH_MAX",262143);
	define("ENTITY_STRINGLENGTH_MAX",1048575);
	define("ENTITY_BLOCKSIZE_DEFAULT",10);
	define("ENTITY_BLOCKSIZE_MAX",1000);
	define("ENTITY_MAXBLOCKSIZE_DEFAULT",100);
	define("ENTITY_MAXBLOCKSIZE_MAX",1000);

	class CEntityDefinitions
	{
		private $m_arrayData;
		private $m_bValid;
		
		function __construct()
		{
			$this->m_bValid = false;
			$this->m_arrayData = array();
		}

		function SetEntityDefinitions($arrayData)
		{
			if (!is_array($arrayData))
			{
				$this->m_arrayData = array();
				return false;	
			}
			$this->m_arrayData = $arrayData;
			return true;
		} 
	
		function GetEntityDefinitions()
		{
			return $this->m_arrayData;	
		}
		
		function CheckEntityDefinitions(&$arrayErrors,$nCheckMode = ENTITYDEF_CORE)
		{
			$bResult = true;
			foreach ($this->m_arrayData as $strEntity => $arrayEntity)
			{
				$bResultEntity = $this->CheckEntityDefinition($strEntity,$arrayErrors,$nCheckMode);
				if ($bResultEntity == false)
				{
					$bResult = false;	
				}
			}
			$this->m_bValid = $bResult;
			return $bResult;	
		}
		
		
		function CheckEntityDefinition($strEntity,&$arrayErrors,$nCheckMode = ENTITYDEF_CORE)
		{
			$arrayEntityDefinition = $this->GetEntityDefinition($strEntity);
			if ($arrayEntityDefinition == false)
			{
				$arrayErrors[] = "The entity \"$strEntity\" is not defined.";
				return false;	
			}			
					
			
			$bResult = true;	
			
			$arrayEntityDefinitionKeys = array(
				"dbtablename",
				"indexattribute",
				"default",
				"indexstate",
				"view",
				"list",
				"userfriendlyattribute",
				"noitemsfoundmessage",
				"blocksize",
				"maxblocksize",
				"showreadonlyinmodify",
				"deletewithoutconfirmation",
				"userfriendlyexportcolumnheaders",
				"webservice",
				"sorting",
				"filteroptions",
				"uniqueindices",
				"attributes",
				"states",
				"tabs",
				"tasks",
				"conditions",
				"textarealines"
				);
			
			$arrayAttributeKeys = array(
				"webserviceparam",
				"exportcolumnheader",
				"sortoption",
				"filteroption",
				"sharecolumn",
				"type",
				"dbtype",
				"control",
				"default",
				"maxlen",
				"minlen",
				"max",
				"min",
				"floatprecision",
				"zerounset", 
				"inputrequired", 
				"charset",
				"enum",
				"tidpattern",
				"enumsort",
				"foreignkey",
				"selectfromwebservice",
				"tableheader",
				"formlabel",
				"readonly",
				"readonlyfrontend",
				"excludefromget",
				"excludefromlist",
				"hideinlist",
				"hideinview",
				"hideinexport",
				"nostorage",
				"indexlink",
				"tab",
				"hidenooption",
				"contextfilter",
				"textarealines");

			$arrayViewKeys = array(
				"refresh",
				"style",
				"hideempty",
				"hideheader",
				"headerlevel",
				"headerstyle");

			$arrayViewStyles = array(
				"",
				"vertical",
				"horizontal",
				"custom",
				"text");
				
			$arrayListKeys = array(
				"refresh");
				

			$arraySortingKeys = array(
				"defaultattribute",
				"defaultorder");

			$arrayFilterOptionKeys = array(
				"submitbuttontext",
				"reset",
				"reseticon",
				"resettooltip",
				"formstyle",
				"formid",
				"filters");
				
			$arrayFilterTypes = array(
				"search",
				"select",
				"checkbox");
				
			$arraySupportedAttributeTypes = array(
				"numericid",
				"integer",
				"float",
				"bool",
				"string",
				"password",
				"date",
				"datetime",
				"enum",
				"sortorder",
				"email",
				"language",
				"currency",
				"foreignkey");
	
			$arraySupportedAttributeDBTypes = array(
				"",
				"string",
				"enum");			

			$arraySupportedAttributeControls = array(
				"",
				"textarea",
				"select");
				
			foreach ($arrayEntityDefinition as $strKey => $strValue)
			{
				if (!ArrayValueExists($arrayEntityDefinitionKeys,$strKey))
				{
					$arrayErrors[] = "$strEntity: The key \"$strKey\" is not allowed.";
					$bResult = false;
				}	
			}
	
	
			// Database Table Name
		
			if ($nCheckMode == ENTITYDEF_WEBSERVICE)
			{
				require_once("wdk_mysql.inc");
				$strDbTableName = ArrayGetValue($arrayEntityDefinition,"dbtablename");
				if ($strDbTableName != "")
				{
					if (SQL_IsValidTableName($strDbTableName) == false)
					{
						$arrayErrors[] = "$strEntity: The database table name \"$strDbTableName\" is not allowed.";
						$bResult = false;
					}
				}
			}


	
			// Web service
	
			$arrayWebserviceConfig = ArrayGetValue($arrayEntityDefinition,"webservice");
			if (!is_array($arrayWebserviceConfig))
			{
				$arrayErrors[] = "$strEntity: No web service configuration found for item type \"$strEntity\".";
				$bResult = false;
			}
			
			$strEntityTag = ArrayGetValue($arrayWebserviceConfig,"itemtag");
			if ($strEntityTag == "")
			{
				$arrayErrors[] = "$strEntity: Web service itemtag is not set.";
				$bResult = false;
			}
			if ($nCheckMode == ENTITYDEF_MODULE)
			{
				$arrayStates = ArrayGetValue($arrayEntityDefinition,"states");
				if (is_array($arrayStates))
				{
					if (ArrayValueExists(ArrayGetValue($arrayEntityDefinition,"states"),"add"))
					{
						$strNewItemIDTag = ArrayGetValue($arrayWebserviceConfig,"newitemidtag");
						if ($strNewItemIDTag == "")
						{
							$arrayErrors[] = "$strEntity: Web service new item id tag is not set.";
							$bResult = false;
						}
					}
				}
			}
			if ($nCheckMode == ENTITYDEF_WEBSERVICE)
			{
				$arrayCommandAdd = ArrayGetValue($arrayEntityDefinition,"webservice","commands","add");
				if (is_array($arrayCommandAdd))
				{
					$strNewItemIDTag = ArrayGetValue($arrayWebserviceConfig,"newitemidtag");
					if ($strNewItemIDTag == "")
					{
						$arrayErrors[] = "$strEntity: Web service new item id tag is not set.";
						$bResult = false;
					}
				}
			}
			
			
			// Blocksize

			if ($nCheckMode == ENTITYDEF_MODULE)			
			{
				$nBlocksize = intval(ArrayGetValue($arrayEntityDefinition,"blocksize"));
				if ($nBlocksize > ENTITY_BLOCKSIZE_MAX)
				{
					$arrayErrors[] = "$strEntity: Block size must not be greater than ".ENTITY_BLOCKSIZE_MAX.".";
					$bResult = false;
				} 
			}


			$nMaxBlocksize = intval(ArrayGetValue($arrayEntityDefinition,"maxblocksize"));
			if ($nMaxBlocksize > ENTITY_MAXBLOCKSIZE_MAX)
			{
				$arrayErrors[] = "$strEntity: Maximum block size must not be greater than ".ENTITY_MAXBLOCKSIZE_MAX.".";
				$bResult = false;
			}
			if ($nMaxBlocksize == 0)
			{
				$nMaxBlocksize = ENTITY_MAXBLOCKSIZE_DEFAULT;
			}
			
			if ($nCheckMode == ENTITYDEF_MODULE)			
			{
				if ($nBlocksize > $nMaxBlocksize)
				{
					$arrayErrors[] = "$strEntity: Block size ($nBlocksize) must not be greater than maximum blocksize ($nMaxBlocksize).";
					$bResult = false;
				}
			}

			
			// Attributes
			
			$arrayFilterWebserviceParams = array();
			
			$arrayAttributes = ArrayGetValue($arrayEntityDefinition,"attributes");
			if (!is_array($arrayAttributes))
			{
				$arrayErrors[] = "$strEntity: No attributes defined for item type \"$strEntity\".";
				$bResult = false;
			}
			else
			{
				if (ArrayCount($arrayAttributes) < 1)
				{
					$arrayErrors[] = "$strEntity: No attributes defined for item type \"$strEntity\".";
					$bResult = false;
				}

				if ($nCheckMode == ENTITYDEF_MODULE)
				{				 
					$strUserFriendlyAttribute = ArrayGetValue($arrayEntityDefinition,"userfriendlyattribute");
					if (ArrayKeyExists($arrayAttributes,$strUserFriendlyAttribute) == false)
					{ 
						$arrayErrors[] = "$strEntity: The user friendly attribute \"$strUserFriendlyAttribute\" is not found in attribute array.";
						$bResult = false;
					}
				}
				$strIndexAttribute = ArrayGetValue($arrayEntityDefinition,"indexattribute");
				if (ArrayKeyExists($arrayAttributes,$strIndexAttribute) == false)
				{
					$arrayErrors[] = "$strEntity: The index attribute \"$strIndexAttribute\" is not defined in the attribute array.";
					$bResult = false;
				}
				
	
				foreach ($arrayAttributes as $strAttribute => $arrayAttribute)
				{
					if (is_int($strAttribute))
					{
						$arrayErrors[] = "$strEntity: \"$strAttribute\" is an integer. The attributes array must use the attribute ids as its keys.";
						$bResult = false;	
					}
					if (StringCheckCharSet($strAttribute,CHARSET_ATTRIBUTENAME) == false)
					{
						$arrayErrors[] = "$strEntity: \"$strAttribute\" contains invalid characters.";
						$bResult = false;	
					}
					foreach ($arrayAttribute as $strKey => $strValue)
					{
						if (!ArrayValueExists($arrayAttributeKeys,$strKey))
						{
							$arrayErrors[] = "$strEntity: The key \"$strKey\" is not allowed.";
							$bResult = false;	
						}	
					}
					
					$bReadOnly = GetBoolValue(ArrayGetValue($arrayAttribute,"readonly"));
					if (!$bReadOnly)
					{
						if ($nCheckMode == ENTITYDEF_MODULE)
						{

						}
					}
					$bNoStorage = GetBoolValue(ArrayGetValue($arrayAttribute,"nostorage"));
					if ($bNoStorage == true)
					{
						if ($strAttribute == $strIndexAttribute)
						{
							$arrayErrors[] = "$strEntity: Index attribute \"$strAttribute\" must not set \"nostorage\".";
							$bResult = false;	
						}	
					}
					$bForeignKey = is_array(ArrayGetValue($arrayAttribute,"foreignkey"));
					$bHideInList = GetBoolValue(ArrayGetValue($arrayAttribute,"hideinlist"));
					$bHideInView = GetBoolValue(ArrayGetValue($arrayAttribute,"hideinview"));

					$strType = ArrayGetValue($arrayAttribute,"type");
					if (!ArrayValueExists($arraySupportedAttributeTypes,$strType))
					{  
						$arrayErrors[] = "$strEntity: Type \"$strType\" defined for attribute \"$strAttribute\" is not supported.";
						$bResult = false;	
					}
					
					$strDBType = ArrayGetValue($arrayAttribute,"dbtype");
					if (!ArrayValueExists($arraySupportedAttributeDBTypes,$strDBType))
					{  
						$arrayErrors[] = "$strEntity: DB Type \"$strDBType\" defined for attribute \"$strAttribute\" is not supported.";
						$bResult = false;	
					}
					
					$strControl = ArrayGetValue($arrayAttribute,"control");
					if (!ArrayValueExists($arraySupportedAttributeControls,$strControl))
					{  
						$arrayErrors[] = "$strEntity: Control \"$strControl\" defined for attribute \"$strAttribute\" is not supported.";
						$bResult = false;	
					}
					
					$strDefault = ArrayGetValue($arrayAttribute,"default");
					
					if ($strType == "integer")
					{
						$nMax = intval(ArrayGetValue($arrayAttribute,"max"));
						if ($bForeignKey == true)
						{
							if ($nMax != 0)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a foreign key reference, hence max must NOT be specified.";
								$bResult = false;										
							}	
						}
						else
						{
							if ($nMax == 0)
							{
								$arrayErrors[] = "$strEntity: Max value MUST be specified for integer attribute \"$strAttribute\".";
								$bResult = false;	
							}
						}
					}
					if ($strType != "float")
					{
						if (ArrayKeyExists($arrayAttribute,"floatprecision"))
						{
							$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" has a decimal places (\"floatprecision\") setting, but is not a float!";
							$bResult = false;	
						}
					}
					if ($strType == "float")
					{
						$nPrecision = intval(ArrayGetValue($arrayAttribute,"floatprecision"));
						if ($nPrecision < 0)
						{
							$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" precision must be 0 or higher.";
							$bResult = false;										
						}
						if ($nPrecision > 10)
						{
							$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" precision must not be higher than 10.";
							$bResult = false;
						}
						$fMin = floatval(ArrayGetValue($arrayAttribute,"min"));
						$fMax = floatval(ArrayGetValue($arrayAttribute,"max")); 
						if ($bForeignKey == true)
						{
							if ($fMin != 0)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a foreign key reference, hence min must NOT be specified.";
								$bResult = false;										
							}	
							if ($fMax != 0)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a foreign key reference, hence max must NOT be specified.";
								$bResult = false;										
							}	
						}
						else
						{
							if ($fMin >= $fMax)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\": min must not be greater or equal to max.";
								$bResult = false;							
							}
						}
					}
					if ($strType == "string")
					{
						$nMaxLen = intval(ArrayGetValue($arrayAttribute,"maxlen"));
						if ($bForeignKey == true)
						{
							if ($nMaxLen != 0)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a foreign key reference, hence maxlen must NOT be specified.";
								$bResult = false;										
							}	
						}
						else
						{
							if ($nMaxLen < 1)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a string, hence maxlen must be specified.";
								$bResult = false;										
							}	
							if ($nMaxLen > ENTITY_STRINGLENGTH_MAX)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a string. maxlen is set to $nMaxLen but maxlen must not be more than ".ENTITY_STRINGLENGTH_MAX;
								$bResult = false;										
							}	
						}
					}

					if ($strType == "password")
					{
						$nMaxLen = intval(ArrayGetValue($arrayAttribute,"maxlen"));
						if ($bForeignKey == true)
						{
							if ($nMaxLen != 0)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a foreign key reference, hence maxlen must NOT be specified.";
								$bResult = false;										
							}	
						}
						else
						{
							if ($nMaxLen != 0)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a password, hence maxlen must NOT be specified.";
								$bResult = false;										
							}	
							$nMinLen = intval(ArrayGetValue($arrayAttribute,"minlen"));
							if ($nMinLen != 0)
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is a password, hence minlen must NOT be specified.";
								$bResult = false;										
							}	
						}
					}

					if ($strType == "enum")
					{
						$arrayEnum = ArrayGetValue($arrayAttribute,"enum");
						if (!is_array($arrayEnum))
						{
							$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" has type \"enum\", but this requires the \"enum\" array to be set!";
							$bResult = false;	
						}
						else
						{
							if (!ArrayValueExists($arrayEnum,$strDefault))
							{
								$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" defaults to \"$strDefault\", but the value is not in the enum array!"; 
								$bResult = false;	
							}	
						}
					}


					$bWebserviceParam = GetBoolValue(ArrayGetValue($arrayAttribute,"webserviceparam"));

					$bSortOption = GetBoolValue(ArrayGetValue($arrayAttribute,"sortoption"));
					if ($bSortOption == true)
					{
/*						if ($bWebserviceParam == false)
						{
							$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is configured to be sortable, but this requires webserviceparam to be true!";
							$bResult = false;	
						}*/
					}

					$bFilterOption = GetBoolValue(ArrayGetValue($arrayAttribute,"filteroption"));
					if ($bFilterOption == true)
					{
						$arrayFilterWebserviceParams[] = $strAttribute;
					}
					
					$bContextFilter = GetBoolValue(ArrayGetValue($arrayAttribute,"contextfiler"));
					if ($bContextFilter == true)
					{
						if ($bFilterOption == false)
						{
							$arrayErrors[] = "$strEntity: Attribute \"$strAttribute\" is configured to be a context filter, but this requires filteroption to be set, too!";
							$bResult = false;	
						}	
					}
				}
			}

			// View Options
			
			if ($nCheckMode == ENTITYDEF_MODULE)
			{
				$arrayView = ArrayGetValue($arrayEntityDefinition,"view");
				if (is_array($arrayView))
				{
					foreach ($arrayView as $strKey => $strValue)
					{
						if (!ArrayValueExists($arrayViewKeys,$strKey))
						{
							$arrayErrors[] = "$strEntity: Setting \"$strKey\" is not allowed in the view section.";
							$bResult = false;	
						}
					}
								
					$strViewStyle = ArrayGetValue($arrayView,"style");
					if (!ArrayValueExists($arrayViewStyles,$strViewStyle))
					{
						$arrayErrors[] = "$strEntity: viewstyle \"$strViewStyle\" is not supported.";
						$bResult = false;	
					}
				}
			}


			// List Options
			
			if ($nCheckMode == ENTITYDEF_MODULE)
			{
				$arrayList = ArrayGetValue($arrayEntityDefinition,"list");
				if (is_array($arrayList))
				{
					foreach ($arrayList as $strKey => $strValue)
					{
						if (!ArrayValueExists($arrayListKeys,$strKey))
						{
							$arrayErrors[] = "$strEntity: Setting \"$strKey\" is not allowed in the list section.";
							$bResult = false;	
						}
					}
				}
			}

			
			// Tasks and States
			
			$arrayTasks = ArrayGetValue($arrayEntityDefinition,"tasks");
			$arrayStates = ArrayGetValue($arrayEntityDefinition,"states");
			if (is_array($arrayTasks))
			{
				foreach ($arrayTasks as $strTask => $arrayTask)
				{
					$strTaskState = ArrayGetValue($arrayTask,"state");
					if (!ArrayValueExists($arrayStates,$strTaskState))
					{
						$arrayErrors[] = "$strEntity: The task \"$strTask\" refers to the state \"$strTaskState\" which is not included in the states list.";
						$bResult = false;	
					}
				}
			}
			$strIndexState = ArrayGetValue($arrayEntityDefinition,"indexstate");
			if ($strIndexState != "")
			{
				if (!is_array($arrayStates))
				{
					$arrayErrors[] = "$strEntity: indexstate refers to a state but no supported states have been defined.";
					$bResult = false;	
					
				}
				else if (!ArrayValueExists($arrayStates,$strIndexState))
				{
					$arrayErrors[] = "$strEntity: indexstate refers to the state \"$strIndexState\" which is not included in the states list.";
					$bResult = false;	
				}
			}
			
			if ($nCheckMode == ENTITYDEF_MODULE)
			{
				$strNoItemsFoundMessage = ArrayGetValue($arrayEntityDefinition,"noitemsfoundmessage");
				if ($strNoItemsFoundMessage == "")
				{
					$arrayErrors[] = "$strEntity: noitemsfoundmessage is empty.";
					$bResult = false;	
				}
			}
			
			// Filter Options
			
			$arrayFilterOptions = ArrayGetValue($arrayEntityDefinition,"filteroptions");
			if (is_array($arrayFilterOptions))
			{
				foreach ($arrayFilterOptions as $strKey => $strValue)
				{
					if (!ArrayValueExists($arrayFilterOptionKeys,$strKey))
					{
						$arrayErrors[] = "$strEntity: Setting \"$strKey\" is not allowed in the filteroptions section.";
						$bResult = false;	
					}
				}

				$arrayFilters = ArrayGetValue($arrayFilterOptions,"filters");
				if (!is_array($arrayFilters))
				{
					$arrayErrors[] = "$strEntity: Setting \"filters\" must be an array.";
					$bResult = false;	
				}
				foreach ($arrayFilters as $strFilter => $arrayFilter)
				{
					$strType = ArrayGetValue($arrayFilter,"type");
					if (!ArrayValueExists($arrayFilterTypes,$strType))
					{
						$arrayErrors[] = "$strEntity: Filter type \"$strType\" is not supported.";
						$bResult = false;	
					}
					$strWebserviceFilterOption = ArrayGetValue($arrayFilter,"webservicefilteroption");
					if ($strWebserviceFilterOption == "")
					{
						$arrayErrors[] = "$strEntity: Filter \"$strFilter\" has not specified a web service filter option.";
						$bResult = false;
					}
					if (!ArrayValueExists($arrayFilterWebserviceParams,$strWebserviceFilterOption))
					{
						$arrayErrors[] = "$strEntity: Filter \"$strFilter\" has specified an undefined web service filter param: \"$strWebserviceFilterOption\"";
						$bResult = false;
					}
				}
				
			}
			
			// Sorting
			
			$arraySorting = ArrayGetValue($arrayEntityDefinition,"sorting");
			if (is_array($arraySorting))
			{
				foreach ($arraySorting as $strKey => $value)
				{
					if (!ArrayValueExists($arraySortingKeys,$strKey))
					{
						$arrayErrors[] = "$strEntity: The key \"$strKey\" is not allowed.";
						$bResult = false;
					}	
				}
				
				$strDefaultAttribute = ArrayGetValue($arraySorting,"defaultattribute");
				$arrayAttribute = ArrayGetValue($arrayAttributes,$strDefaultAttribute);
				if (!is_array($arrayAttribute))
				{
					$arrayErrors[] = "$strEntity: An invalid default sorting attribute was specified: \"$strDefaultAttribute\".";
					$bResult = false;
				}
				$strDefaultOrder = ArrayGetValue($arraySorting,"defaultorder");
				if ($strDefaultOrder != "")
				{
					if (($strDefaultOrder != "asc") && ($strDefaultOrder != "desc"))
					{
						$arrayErrors[] = "$strEntity: Default order must be \"asc\" or \"desc\"!";
						$bResult = false;						
					}
				}
			}
			
			
			// Unique Indices
			$arrayUniqueIndices = ArrayGetValue($arrayEntityDefinition,"uniqueindices");
			if (is_array($arrayUniqueIndices))
			{
				foreach ($arrayUniqueIndices as $strNameOfIndex => $arrayUniqueIndex)
				{
					if (StringCheckCharSet($strNameOfIndex,CHARSET_ATTRIBUTENAME) == false)
					{
						$arrayErrors[] = "$strEntity: Unique index \"$strNameOfIndex\" contains invalid characters.";
						$bResult = false;								
					}
					else
					{
						if (!is_array($arrayUniqueIndex))
						{
							$arrayErrors[] = "$strEntity: \"uniqueindices\" must consist of arrays only!";
							$bResult = false;						
						}
						else
						{
							if (ArrayCount($arrayUniqueIndex) == 0)
							{
								$arrayErrors[] = "$strEntity: A unique index definition array must not be empty.";
								$bResult = false;															
							}
							else
							{
								$arrayUniqueIndexTemp = array();
								foreach ($arrayUniqueIndex as $strUniqueIndexAtribute)
								{
									if (!ArrayKeyExists($arrayAttributes,$strUniqueIndexAtribute))
									{
										$arrayErrors[] = "$strEntity: \"$strUniqueIndexAtribute\" is defined in an unique index but is not defined in the attribute definition array.";
										$bResult = false;															
									}
									else
									{
										if (ArrayValueExists($arrayUniqueIndexTemp,$strUniqueIndexAtribute))
										{
											$arrayErrors[] = "$strEntity: \"$strUniqueIndexAtribute\" is used at leasdt twice within at least one uniqueindex definition.";
											$bResult = false;															
										}
										else
										{
											$arrayUniqueIndexTemp[] = $strUniqueIndexAtribute;
										}
									}
								}
							}
						}
					}
				}
			}

			return $bResult;
		}
		
		
		function AssertValid()
		{
			return $this->m_bValid;	
		}

		function AssertValidEntity($strEntity)
		{
			if (!$this->m_bValid)
			{
				return false;
			}
			return ArrayKeyExists($this->m_arrayData,$strEntity);
		}

		
		function GetEntityDefinition($strEntity)
		{
			$arrayEntity = ArrayGetValue($this->m_arrayData,$strEntity);
			if (!is_array($arrayEntity))
			{
				return false;
			}
			return $arrayEntity;
		}
		
		function GetAttributes($strEntity)
		{
			$arrayAttributes = ArrayGetValue($this->m_arrayData,$strEntity,"attributes");
			if (!is_array($arrayAttributes))
			{
				return false;
			}
			return $arrayAttributes;
		}
		
		function GetDatabaseTableName($strEntity)
		{
			return ArrayGetValue($this->m_arrayData,$strEntity,"dbtablename");
		}
		
		function GetWebserviceParamName($strEntity,$strAttribute)
		{
			return $strAttribute;	
		}

		function GetFieldName($strEntity,$strAttribute)
		{
			return StringUpperCase($strAttribute);
		}
		
	}



