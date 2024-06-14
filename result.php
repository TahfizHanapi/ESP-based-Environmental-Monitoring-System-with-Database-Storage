<?php
header('Content-Type: application/json');

$link = mysqli_connect('localhost', 'root', 'Tahfiz_2002', 'environmentcensor');

if (!$link) {
    die('Could not connect: ' . mysqli_error($link));
}

$query = "SELECT temperature, humidity, distance, timestamp FROM environment ORDER BY timestamp DESC LIMIT 100";
$result = mysqli_query($link, $query);

$data = array();

while ($row = mysqli_fetch_assoc($result)) {
    $data[] = $row;
}

echo json_encode($data);

mysqli_close($link);
?>
