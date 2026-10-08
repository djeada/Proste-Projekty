// User interface: asks for a city, downloads its weather from wttr.in and prints the report.
const readline = require('node:readline');
const { buildUrl, parseWeather, formatReport } = require('./weather.js');

const TIMEOUT_MS = 10000;

function ask(question) {
  const rl = readline.createInterface({ input: process.stdin, output: process.stdout });
  return new Promise((resolve) => {
    rl.question(question, (answer) => {
      resolve(answer);
      rl.close();
    });
    rl.on('close', () => resolve(''));
  });
}

async function getReport(city) {
  let response;
  try {
    response = await fetch(buildUrl(city), { signal: AbortSignal.timeout(TIMEOUT_MS) });
  } catch (error) {
    const reason = (error.cause && error.cause.code) || error.message;
    throw new Error(`Network error: could not reach wttr.in (${reason})`);
  }
  if (!response.ok) {
    // wttr.in answers HTTP 500 for places it does not know
    throw new Error(`City not found: ${city} (HTTP ${response.status})`);
  }
  let weather;
  try {
    weather = parseWeather(await response.text());
  } catch (error) {
    throw new Error(`Empty or unexpected response from wttr.in for: ${city}`);
  }
  return formatReport(weather, city);
}

async function main() {
  const fromArgs = process.argv.slice(2).join(' ').trim();
  const city = fromArgs || (await ask('City or postal code: ')).trim();
  if (!city) {
    console.error('Please enter a city name or postal code.');
    process.exitCode = 1;
    return;
  }
  try {
    console.log(await getReport(city));
  } catch (error) {
    console.error(error.message);
    process.exitCode = 1;
  }
}

main();
