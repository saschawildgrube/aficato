$this->IncludeElement('linechart');

$linechart = new CLineChartElement($this->GetWebsite(),"default");

$arrayData = array(
	array(
		'month' => 'January',
		'blue' => 2,
		'red' => 1),
	array(
		'month' => 'February',
		'blue' => 5,
		'red' => 6),
	array(
		'month' => 'March',
		'blue' => 15,
		'red' => 18), 
	array(
		'month' => 'April',
		'blue' => 12,
		'red' => 7)
	);
	
$arrayConfig = array(
	'legend' => 'month',
	'lines' => array(
		array(
			'key' => 'blue',
			'label' => 'Blue',
			'color' => 'blue',
			'weight' => 2
			),
		array(
			'key' => 'red',
			'label' => 'Red',
			'color' => 'red',
			'weight' => 2
			)
		)
	);


$strOutput = $linechart->RenderLineChart(
	$arrayData,
	$arrayConfig);