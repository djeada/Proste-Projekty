// Weather logic: build the wttr.in URL, parse its JSON reply and format the report.

const RESET = '\x1b[0m';
const BOLD_CYAN = '\x1b[1;36m';
const YELLOW = '\x1b[33m';
const FORECAST_DAYS = 3;
const NOON_SLOT = 4; // wttr.in gives eight 3-hour slots per day; index 4 is 12:00

function buildUrl(city) {
  return `https://wttr.in/${encodeURIComponent(city)}?format=j1`;
}

function integer(value) {
  const number = parseInt(value, 10);
  if (Number.isNaN(number)) {
    throw new Error(`not a number: ${value}`);
  }
  return number;
}

// Throws if the reply is not JSON or has no weather data.
function parseWeather(text) {
  try {
    const data = JSON.parse(text);
    const current = data.current_condition[0];
    const days = data.weather;
    return {
      temp: integer(current.temp_C),
      feelsLike: integer(current.FeelsLikeC),
      description: current.weatherDesc[0].value.trim(),
      humidity: integer(current.humidity),
      windSpeed: integer(current.windspeedKmph),
      windDir: current.winddir16Point,
      pressure: integer(current.pressure),
      sunrise: days[0].astronomy[0].sunrise,
      sunset: days[0].astronomy[0].sunset,
      forecast: days.slice(0, FORECAST_DAYS).map((day) => ({
        date: day.date,
        minTemp: integer(day.mintempC),
        maxTemp: integer(day.maxtempC),
        description: day.hourly[NOON_SLOT].weatherDesc[0].value.trim(),
      })),
    };
  } catch (error) {
    throw new Error('unexpected weather data');
  }
}

function formatReport(weather, city) {
  const lines = [
    `${BOLD_CYAN}Weather in ${city}${RESET}`,
    `  Conditions   ${weather.description}`,
    `  Temperature  ${YELLOW}${weather.temp} °C${RESET} (feels like ${weather.feelsLike} °C)`,
    `  Humidity     ${weather.humidity} %`,
    `  Wind         ${weather.windSpeed} km/h ${weather.windDir}`,
    `  Pressure     ${weather.pressure} hPa`,
    `  Sunrise      ${weather.sunrise}   Sunset ${weather.sunset}`,
    '',
    `${BOLD_CYAN}Forecast${RESET}`,
  ];
  for (const day of weather.forecast) {
    const range = `${String(day.minTemp).padStart(3)} / ${String(day.maxTemp).padStart(3)} °C`;
    lines.push(`  ${day.date}  ${YELLOW}${range}${RESET}  ${day.description}`);
  }
  return lines.join('\n');
}

module.exports = { buildUrl, parseWeather, formatReport };
