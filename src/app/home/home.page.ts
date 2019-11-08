import {Component, OnDestroy, OnInit} from '@angular/core';
import {Observable} from 'rxjs';
import {HttpClient} from '@angular/common/http';
import 'rxjs-compat/add/operator/map';
import 'rxjs-compat/add/observable/interval';
import 'rxjs-compat/add/operator/timeInterval';

@Component({
    selector: 'app-home',
    templateUrl: 'home.page.html',
    styleUrls: ['home.page.scss'],
})
export class HomePage implements OnInit, OnDestroy {
    locationKey = '249758'; // Location is Amsterdam
    apiKey = '9TjkPVk3Bz0jj3cMiki1NFvChzXs4WU0';
    apiURL = 'http://dataservice.accuweather.com/forecasts/v1/hourly/1hour/';
    getForecastData: Observable<any>;
    getAudioData: Observable<any>;
    audioActive: any;
    audioStatus: string;
    audio: any;
    audioName: any;
    weatherDegree: string;
    weatherCondition: string;
    backgroundImage: string;

    constructor(public http: HttpClient) {
        this.getForecastData = this.http.get(this.apiURL + this.locationKey + '?apikey=' + this.apiKey);
        this.getForecastData.subscribe(data => {
            // console.log(data[0].Temperature.Value);
            // if (data[0].Temperature.Value > 0 || data[0].Temperature.Value < 60) {
            //     this.playAudio();
            // }

            this.weatherDegree = JSON.parse(JSON.stringify(data[0].Temperature.Value));
            this.weatherCondition = JSON.parse(JSON.stringify(data[0].IconPhrase));

            this.updateBackground();
        });

        this.updatePage();
    }

    ngOnInit() {
        this.audio = new Audio();
        this.audio.src = '../../assets/audio/chillhop-essentials.mp3';
        this.audio.load();
    }

    playAudio() {
        this.audio.play().then((success) => {
            this.audioName = this.audio.src.substring(35);
            console.log('success');
        }, (error) => {
            console.log('error');
        });
        this.audio.loop = true;
    }

    stopAudio() {
        this.audio.pause();
    }

    ngOnDestroy() {
        if (this.audio) {
            this.audio.pause();
            this.audio = null;
        }
    }

    updatePage() {
        Observable.interval(1000).timeInterval().subscribe(() => {
            this.getAudioData = this.http.get('http://localhost/audio_data.php');
            this.getAudioData.subscribe(data => {
                this.audioActive = data[0].audio_active;
                if (this.audioActive === '1') {
                    this.playAudio();
                    this.audioStatus = 'Now Playing';
                } else if (this.audioActive === '0') {
                    this.stopAudio();
                    this.audioStatus = 'Paused';
                }
            });
        });

    }

    updateBackground() {
        if (this.weatherCondition === 'Showers' || this.weatherCondition === 'Rain') {
            this.backgroundImage = 'https://bit.ly/2WNrH1T';
        } else if (this.weatherCondition === 'Sunny') {
            this.backgroundImage = 'https://bit.ly/34vxWu2';
        } else if (this.weatherCondition === 'Mostly cloudy') {
            this.backgroundImage = 'https://bit.ly/2CglFxr';
        } else if (this.weatherCondition === 'Intermittent clouds' || this.weatherCondition === 'Partly cloudy') {
            this.backgroundImage = 'https://bit.ly/34z1XJy';
        } else if (this.weatherCondition === 'Cloudy') {
            this.backgroundImage = 'https://bit.ly/2WJfkUP';
        }
    }
}
