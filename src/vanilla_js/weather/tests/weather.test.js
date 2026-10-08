// Tests for the weather logic, using a saved wttr.in reply (no network needed).
const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const { buildUrl, parseWeather, formatReport } = require('../src/weather.js');

const sample = fs.readFileSync(path.join(__dirname, 'warszawa.json'), 'utf8');

test('buildUrl for a plain city', () => {
  assert.equal(buildUrl('Warszawa'), 'https://wttr.in/Warszawa?format=j1');
});

test('buildUrl encodes spaces and Polish letters', () => {
  assert.equal(buildUrl('Kraków Łódź'), 'https://wttr.in/Krak%C3%B3w%20%C5%81%C3%B3d%C5%BA?format=j1');
});

test('buildUrl keeps a postal code dash and encodes a slash', () => {
  assert.equal(buildUrl('00-001'), 'https://wttr.in/00-001?format=j1');
  assert.equal(buildUrl('a/b'), 'https://wttr.in/a%2Fb?format=j1');
});

test('parseWeather reads the current conditions', () => {
  const weather = parseWeather(sample);
  assert.equal(weather.temp, 20);
  assert.equal(weather.feelsLike, 15);
  assert.equal(weather.description, 'Overcast');
  assert.equal(weather.humidity, 50);
  assert.equal(weather.windSpeed, 22);
  assert.equal(weather.windDir, 'SSW');
  assert.equal(weather.pressure, 1005);
  assert.equal(weather.sunrise, '06:49 AM');
  assert.equal(weather.sunset, '05:58 PM');
});

test('parseWeather reads the three-day forecast', () => {
  assert.deepEqual(parseWeather(sample).forecast, [
    { date: '2026-10-08', minTemp: 14, maxTemp: 22, description: 'Overcast' },
    { date: '2026-10-09', minTemp: 11, maxTemp: 18, description: 'Overcast' },
    { date: '2026-10-10', minTemp: 12, maxTemp: 14, description: 'Patchy rain nearby' },
  ]);
});

test('parseWeather rejects replies without weather data', () => {
  for (const text of ['', 'location not found', '{"current_condition": [{"temp_C": "20"', '{}']) {
    assert.throws(() => parseWeather(text), /unexpected weather data/);
  }
});

test('formatReport contains the values', () => {
  const report = formatReport(parseWeather(sample), 'Warszawa');
  assert.match(report, /Weather in Warszawa/);
  assert.match(report, /20 °C/);
  assert.match(report, /feels like 15 °C/);
  assert.match(report, /1005 hPa/);
  assert.match(report, /2026-10-10/);
  assert.match(report, /Patchy rain nearby/);
});
