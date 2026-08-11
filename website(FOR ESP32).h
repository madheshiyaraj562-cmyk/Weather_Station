#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <Arduino.h>

const char webpage[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Atmosphere Executive Console</title>
<link href="https://fonts.googleapis.com/css2?family=Cinzel:wght@500;700&family=Plus+Jakarta+Sans:wght@300;400;500;600;700&display=swap" rel="stylesheet">
<style>
  :root {
    --bg: #07090e;
    --card-bg: rgba(18, 24, 38, 0.55);
    --card-border: rgba(255, 255, 255, 0.08);
    --accent: #d4af37;
    --accent-blue: #38bdf8;
    --alert-red: #ef4444;
    --text-primary: #f8fafc;
    --text-secondary: #94a3b8;
  }
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: 'Plus Jakarta Sans', sans-serif;
    background: var(--bg);
    background-image: 
      radial-gradient(at 0% 0%, rgba(212, 175, 55, 0.06) 0px, transparent 50%),
      radial-gradient(at 100% 100%, rgba(56, 189, 248, 0.05) 0px, transparent 50%);
    color: var(--text-primary);
    min-height: 100vh;
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 24px;
  }
  .container { width: 100%; max-width: 720px; display: flex; flex-direction: column; gap: 20px; }
  .card {
    background: var(--card-bg);
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
    border: 1px solid var(--card-border);
    border-radius: 28px;
    padding: 28px;
    box-shadow: 0 30px 60px -12px rgba(0, 0, 0, 0.56);
  }
  
  .hazard-banner {
    background: rgba(239, 68, 68, 0.15);
    border: 1px solid rgba(239, 68, 68, 0.4);
    color: var(--alert-red);
    padding: 14px 20px;
    border-radius: 20px;
    font-size: 13px;
    font-weight: 700;
    letter-spacing: 0.5px;
    display: none;
    align-items: center;
    gap: 12px;
  }

  .header { display: flex; justify-content: space-between; align-items: center; }
  .title-area h1 {
    font-family: 'Cinzel', serif; font-size: 20px; letter-spacing: 2px;
    text-transform: uppercase; color: var(--accent); font-weight: 700;
  }
  .location { font-size: 13px; color: var(--text-secondary); letter-spacing: 0.5px; margin-top: 2px; }
  
  .status-pill {
    display: inline-flex; align-items: center; gap: 8px; background: rgba(212, 175, 55, 0.1);
    border: 1px solid rgba(212, 175, 55, 0.25); padding: 6px 16px; border-radius: 100px;
    font-size: 12px; font-weight: 600; color: var(--accent);
  }
  .pulse-dot {
    width: 6px; height: 6px; background: var(--accent); border-radius: 50%;
    box-shadow: 0 0 10px var(--accent); animation: pulse 2.5s infinite;
  }
  .status-pill.alert-mode { background: rgba(239, 68, 68, 0.1); border-color: rgba(239, 68, 68, 0.4); color: var(--alert-red); }
  .status-pill.alert-mode .pulse-dot { background: var(--alert-red); box-shadow: 0 0 10px var(--alert-red); }

  .hero { display: grid; grid-template-columns: 1.2fr 1fr; gap: 20px; align-items: center; background: linear-gradient(145deg, rgba(255, 255, 255, 0.03), rgba(255, 255, 255, 0.005)); }
  @media (max-width: 580px) { .hero { grid-template-columns: 1fr; text-align: center; } }
  .hero-left { display: flex; flex-direction: column; }
  
  .time-display { display: flex; align-items: baseline; gap: 10px; }
  .time-main { font-size: 38px; font-weight: 300; letter-spacing: -1px; color: var(--text-primary); }
  .time-daypart { font-size: 13px; font-weight: 600; color: var(--accent); text-transform: uppercase; letter-spacing: 1px; }
  
  .date-display { font-size: 13px; color: var(--text-secondary); margin-bottom: 16px; font-weight: 500; }
  .temp-main { font-size: 72px; font-weight: 200; line-height: 0.9; letter-spacing: -3px; color: #ffffff; }
  .temp-main span { font-size: 36px; color: var(--accent); font-weight: 300; }
  .feels-like { font-size: 13px; color: var(--text-secondary); margin-top: 8px; }
  
  .hero-right { display: flex; flex-direction: column; align-items: flex-end; gap: 14px; }
  @media (max-width: 580px) { .hero-right { align-items: center; } }
  .condition-badge { font-size: 13px; font-weight: 600; color: var(--text-primary); background: rgba(255, 255, 255, 0.05); padding: 8px 16px; border-radius: 100px; border: 1px solid var(--card-border); }

  .sun-widget { background: rgba(255, 255, 255, 0.02); border: 1px solid var(--card-border); padding: 10px 14px; border-radius: 16px; width: 100%; display: flex; justify-content: space-around; font-size: 12px; color: var(--text-secondary); }
  .sun-item { display: flex; align-items: center; gap: 6px; }
  .sun-item b { color: var(--text-primary); font-weight: 600; }

  .music-btn {
    background: linear-gradient(135deg, rgba(212,175,55,0.2), rgba(212,175,55,0.05));
    border: 1px solid rgba(212,175,55,0.4);
    color: var(--accent);
    padding: 8px 16px;
    border-radius: 100px;
    font-size: 12px;
    font-weight: 700;
    cursor: pointer;
    display: inline-flex;
    align-items: center;
    gap: 6px;
    transition: all 0.2s;
  }
  .music-btn:hover { background: rgba(212,175,55,0.3); transform: scale(1.02); }
  
  .grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 14px; }
  @media (max-width: 580px) { .grid { grid-template-columns: repeat(2, 1fr); } }
  .metric-card { background: rgba(18, 24, 38, 0.4); border: 1px solid var(--card-border); border-radius: 20px; padding: 18px; display: flex; flex-direction: column; justify-content: space-between; transition: transform 0.2s, border-color 0.2s; }
  .metric-card:hover { transform: translateY(-2px); border-color: rgba(212, 175, 55, 0.3); }
  .metric-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px; }
  .metric-title { font-size: 12px; font-weight: 500; color: var(--text-secondary); text-transform: uppercase; letter-spacing: 0.8px; }
  .metric-icon { width: 18px; height: 18px; stroke: var(--accent); fill: none; stroke-width: 1.8; }
  .metric-value { font-size: 20px; font-weight: 600; color: var(--text-primary); }
  .metric-sub { font-size: 11px; color: var(--text-secondary); margin-top: 4px; }
  
  /* Controls styling */
  .control-group { display: flex; flex-direction: column; gap: 8px; width: 100%; margin-top: 4px; }
  .control-group label { font-size: 11px; color: var(--text-secondary); text-transform: uppercase; font-weight: 600; }
  input[type="range"] {
    -webkit-appearance: none; width: 100%; height: 6px; background: rgba(255,255,255,0.1); border-radius: 3px; outline: none;
  }
  input[type="range"]::-webkit-slider-thumb {
    -webkit-appearance: none; width: 16px; height: 16px; border-radius: 50%; background: var(--accent-blue); cursor: pointer;
  }
  input[type="color"] {
    -webkit-appearance: none; border: none; width: 100%; height: 32px; border-radius: 8px; cursor: pointer; background: none;
  }
  input[type="color"]::-webkit-color-swatch-wrapper { padding: 0; }
  input[type="color"]::-webkit-color-swatch { border: 1px solid var(--card-border); border-radius: 8px; }

  /* Messenger Box styling */
  .messenger-box {
    background: rgba(10, 14, 23, 0.7);
    border: 1px solid var(--card-border);
    border-radius: 20px;
    padding: 16px;
    display: flex;
    flex-direction: column;
    gap: 10px;
    max-height: 200px;
    overflow-y: auto;
  }
  .message-bubble {
    background: rgba(56, 189, 248, 0.1);
    border: 1px solid rgba(56, 189, 248, 0.2);
    padding: 10px 14px;
    border-radius: 14px;
    font-size: 12px;
    color: var(--text-primary);
    line-height: 1.4;
  }

  .tag { display: inline-block; padding: 2px 8px; border-radius: 6px; font-size: 10px; font-weight: 700; text-transform: uppercase; margin-left: 6px; }
  .tag-good { background: rgba(34, 197, 94, 0.2); color: #4ade80; }
  .tag-mod { background: rgba(234, 179, 8, 0.2); color: #facc15; }
  .tag-high { background: rgba(239, 68, 68, 0.2); color: #f87171; }
  .footer { display: flex; justify-content: space-between; font-size: 12px; color: var(--text-secondary); padding: 0 8px; }
  .footer span b { color: var(--text-primary); font-weight: 600; }
  @keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.3; } }

  #clap-toast { position: fixed; top: 24px; left: 50%; transform: translateX(-50%) translateY(-30px); background: linear-gradient(135deg, rgba(212,175,55,0.95), rgba(180,140,20,0.95)); color: #0a0a0a; font-weight: 700; font-size: 14px; padding: 12px 22px; border-radius: 100px; box-shadow: 0 15px 35px -8px rgba(212, 175, 55, 0.6); display: flex; align-items: center; gap: 10px; opacity: 0; pointer-events: none; transition: opacity 0.3s ease, transform 0.3s ease; z-index: 999; }
  #clap-toast.show { opacity: 1; transform: translateX(-50%) translateY(0); }
</style>
</head>
<body>

<div id="clap-toast">
  <span>👏</span> <span>Clap detected — <span id="clap-toast-time">--:--:--</span></span>
</div>

<div class="container">
  <div id="hazard-banner" class="hazard-banner">
    <span>⚠️</span> <span id="hazard-text"></span>
  </div>

  <div class="card header">
    <div class="title-area">
      <h1>Atmosphere</h1>
      <div class="location">Gorakhpur Observatory, IN</div>
    </div>
    <div class="status-pill" id="status-pill">
      <div class="pulse-dot"></div> <span id="status-text">Live Telemetry</span>
    </div>
  </div>

  <div class="card hero">
    <div class="hero-left">
      <div class="time-display">
        <div class="time-main" id="time">--:--:--</div>
        <div class="time-daypart" id="day-part"></div>
      </div>
      <div class="date-display" id="date">------------------</div>
      <div class="temp-main"><span id="temp">--</span><span>°C</span></div>
      <div class="feels-like">Feels like <b id="feels">--</b>°C</div>
    </div>
    <div class="hero-right">
      <div class="condition-badge" id="condition">Syncing...</div>
      <button class="music-btn" onclick="playMusicTrack()">🎵 Play Track 88</button>

      <div class="sun-widget">
        <div class="sun-item"><span>🌅</span> <div><small style="display:block; font-size:9px">RISE</small><b id="sunrise">--:--</b></div></div>
        <div class="sun-item"><span>🌇</span> <div><small style="display:block; font-size:9px">SET</small><b id="sunset">--:--</b></div></div>
      </div>
    </div>
  </div>

  <div class="grid">
    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">Air Quality</span><svg class="metric-icon" viewBox="0 0 24 24"><path d="M18 10h-1.26A8 8 0 109 20h9a5 5 0 000-10z"/></svg></div>
      <div><div class="metric-value"><span id="aqi">--</span> <span id="aqi-tag" class="tag">--</span></div><div class="metric-sub">PM2.5: <span id="pm25">--</span> µg/m³</div></div>
    </div>

    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">UV Index</span><svg class="metric-icon" viewBox="0 0 24 24"><circle cx="12" cy="12" r="5"/><path d="M12 1v2m0 18v2M4.22 4.22l1.42 1.42m12.72 12.72l1.42 1.42M1 12h2m18 0h2M4.22 19.78l1.42-1.42M18.36 5.64l1.42-1.42"/></svg></div>
      <div><div class="metric-value"><span id="uv">--</span> <span id="uv-tag" class="tag">--</span></div><div class="metric-sub">Solar Exposure</div></div>
    </div>

    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">Humidity</span><svg class="metric-icon" viewBox="0 0 24 24"><path d="M12 2.69l5.66 5.66a8 8 0 11-11.31 0z"/></svg></div>
      <div><div class="metric-value"><span id="humidity">--</span>%</div><div class="metric-sub">Relative Moisture</div></div>
    </div>

    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">Wind</span><svg class="metric-icon" viewBox="0 0 24 24"><path d="M9.59 4.59A2 2 0 1111 8H2m10.59 11.41A2 2 0 1014 16H2m15.73-8.27A2.5 2.5 0 1119.5 12H2"/></svg></div>
      <div><div class="metric-value"><span id="wind">--</span> <small style="font-size: 12px">km/h</small></div><div class="metric-sub">Surface Velocity</div></div>
    </div>

    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">Local Env</span><svg class="metric-icon" viewBox="0 0 24 24"><path d="M18.36 6.64a9 9 0 11-12.73 0M12 2v10"/></svg></div>
      <div>
        <div class="metric-value"><span id="loc_temp">--</span>°C <span style="font-size:12px; font-weight:400; color:var(--text-secondary)">/</span> <span id="loc_hum">--</span>%</div>
        <div class="metric-sub"><span id="pressure">--</span> hPa (<span id="pressure-trend">--</span>)</div>
      </div>
    </div>

    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">ESP32 Core</span><svg class="metric-icon" viewBox="0 0 24 24"><path d="M14 14.76V3.5a2.5 2.5 0 00-5 0v11.26a4.5 4.5 0 105 0z"/></svg></div>
      <div>
        <div class="metric-value"><span id="esp_temp">--</span>°C</div>
        <div class="metric-sub">Processor Silicon Temp</div>
      </div>
    </div>

    <!-- NEW BATTERY PERCENTAGE CARD -->
    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">Battery</span><svg class="metric-icon" viewBox="0 0 24 24"><rect x="1" y="6" width="18" height="12" rx="2"/><path d="M23 10v4"/></svg></div>
      <div>
        <div class="metric-value"><span id="battery">--</span>%</div>
        <div class="metric-sub">System Charge Level</div>
      </div>
    </div>

    <!-- NEW SPEAKER VOLUME KNOB (SLIDER) -->
    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">Volume Knob</span><svg class="metric-icon" viewBox="0 0 24 24"><polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 010 14.14M15.54 8.46a5 5 0 010 7.07"/></svg></div>
      <div class="control-group">
        <label>Level: <span id="vol-val">25</span></label>
        <input type="range" id="vol-slider" min="0" max="50" value="25" oninput="updateVolume(this.value)">
      </div>
    </div>

    <!-- NEW COLOR KNOB (RGB COLOR PICKER) -->
    <div class="metric-card">
      <div class="metric-header"><span class="metric-title">Color Knob</span><svg class="metric-icon" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10"/><path d="M12 2a15.3 15.3 0 014 10 15.3 15.3 0 01-4 10 15.3 15.3 0 01-4-10 15.3 15.3 0 014-10z"/></svg></div>
      <div class="control-group">
        <label>RGB Selector</label>
        <input type="color" id="rgb-picker" value="#d4af37" onchange="updateRGB(this.value)">
      </div>
    </div>
  </div>

  <!-- NEW MESSENGER BOX DISPLAYING DATA TWICE -->
  <div class="card" style="padding: 20px;">
    <div class="metric-header" style="margin-bottom: 12px;"><span class="metric-title">Telemetry Messenger Log</span></div>
    <div class="messenger-box" id="messenger-box">
      <div class="message-bubble">Initializing telemetry communication channel...</div>
    </div>
  </div>

  <div class="footer"><span>Last API Sync: <b id="last">--:--:--</b></span></div>
</div>

<script>
var lastClapId = null;
var clapToastTimer = null;

function playMusicTrack() {
  fetch('/play/88')
  .then(res => res.text())
  .then(txt => {
    var toast = document.getElementById("clap-toast");
    toast.innerHTML = "<span>🎵</span> <span>Playing Track 88 (Music)</span>";
    toast.classList.add("show");
    setTimeout(() => toast.classList.remove("show"), 3000);
  })
  .catch(err => console.error("Music play error:", err));
}

function updateVolume(val) {
  document.getElementById("vol-val").innerText = val;
  fetch('/volume?val=' + val).catch(err => console.error("Volume error:", err));
}

function updateRGB(hex) {
  fetch('/rgb?hex=' + encodeURIComponent(hex)).catch(err => console.error("RGB error:", err));
}

function getWeatherCondition(code) {
  if (code === 0) return "Clear Sky";
  if (code >= 1 && code <= 3) return "Partly Cloudy";
  if (code >= 45 && code <= 48) return "Foggy";
  if (code >= 51 && code <= 67) return "Rain Showers";
  if (code >= 80 && code <= 82) return "Heavy Rain";
  if (code >= 95) return "Thunderstorm";
  return "Clear";
}

function updateTags(data) {
  var aqiTag = document.getElementById("aqi-tag");
  if (data.aqi <= 50) { aqiTag.className = "tag tag-good"; aqiTag.innerText = "Good"; }
  else if (data.aqi <= 150) { aqiTag.className = "tag tag-mod"; aqiTag.innerText = "Moderate"; }
  else { aqiTag.className = "tag tag-high"; aqiTag.innerText = "Hazard"; }

  var uvTag = document.getElementById("uv-tag");
  if (data.uv <= 2) { uvTag.className = "tag tag-good"; uvTag.innerText = "Low"; }
  else if (data.uv <= 5) { uvTag.className = "tag tag-mod"; uvTag.innerText = "Moderate"; }
  else { uvTag.className = "tag tag-high"; uvTag.innerText = "High"; }
}

function updateData(){
  fetch('/api')
  .then(function(res) { return res.json(); })
  .then(function(data) {
    let timeParts = data.time.split(":");
    let h24 = parseInt(timeParts[0]);
    let ampm = h24 >= 12 ? "PM" : "AM";
    let h12 = h24 % 12 || 12;
    let tod = (h24 >= 5 && h24 < 17) ? "Morning" : "Night";

    document.getElementById("time").innerText = h12 + ":" + timeParts[1] + ":" + timeParts[2] + " " + ampm;
    document.getElementById("day-part").innerText = "• " + tod;
    
    document.getElementById("date").innerText = data.date;
    document.getElementById("temp").innerText = data.temp.toFixed(1);
    document.getElementById("feels").innerText = data.feels_like.toFixed(1);
    document.getElementById("humidity").innerText = data.humidity;
    document.getElementById("wind").innerText = data.wind.toFixed(1);
    document.getElementById("uv").innerText = data.uv.toFixed(1);
    document.getElementById("aqi").innerText = data.aqi;
    document.getElementById("pm25").innerText = data.pm25.toFixed(1);
    document.getElementById("condition").innerText = getWeatherCondition(data.weather_code);
    
    document.getElementById("sunrise").innerText = data.sunrise;
    document.getElementById("sunset").innerText = data.sunset;

    document.getElementById("loc_temp").innerText = data.loc_temp.toFixed(1);
    document.getElementById("loc_hum").innerText = data.loc_hum.toFixed(0);
    document.getElementById("esp_temp").innerText = data.esp_temp.toFixed(1);
    document.getElementById("pressure").innerText = data.pressure.toFixed(1);
    document.getElementById("pressure-trend").innerText = data.pressure_trend;
    document.getElementById("battery").innerText = data.battery;
    document.getElementById("last").innerText = data.last_update;

    updateTags(data);

    // Populate Messenger box showing data entries twice
    let msgBox = document.getElementById("messenger-box");
    let summaryText = `[Sync] Temp: ${data.temp.toFixed(1)}°C | Humidity: ${data.humidity}% | AQI: ${data.aqi}`;
    msgBox.innerHTML = `
      <div class="message-bubble">1. ${summaryText}</div>
      <div class="message-bubble">2. ${summaryText}</div>
    `;

    let hazards = [];
    if (data.aqi >= 150) hazards.push("Hazardous Air Quality (" + data.aqi + ").");
    if ((data.weather_code >= 80 && data.weather_code <= 82) || data.weather_code >= 95) hazards.push("Severe Rain/Storms.");
    
    var banner = document.getElementById("hazard-banner");
    var pill = document.getElementById("status-pill");
    
    if (hazards.length > 0) {
      document.getElementById("hazard-text").innerText = hazards.join(" ");
      banner.style.display = "flex";
      pill.classList.add("alert-mode");
      document.getElementById("status-text").innerText = "System Alert";
    } else {
      banner.style.display = "none";
      pill.classList.remove("alert-mode");
      document.getElementById("status-text").innerText = "Live Telemetry";
    }

    if (lastClapId === null) { lastClapId = data.clap_id; }
    else if (data.clap_id !== lastClapId) {
      lastClapId = data.clap_id;
      var toast = document.getElementById("clap-toast");
      document.getElementById("clap-toast-time").innerText = h12 + ":" + timeParts[1] + ":" + timeParts[2] + " " + ampm;
      toast.classList.add("show");
      if(clapToastTimer) clearTimeout(clapToastTimer);
      clapToastTimer = setTimeout(() => toast.classList.remove("show"), 3500);
    }
  })
  .catch(function(err) { console.error("API Error:", err); });
}

setInterval(updateData, 1000);
updateData();
</script>
</body>
</html>
)HTML";
#endif 
