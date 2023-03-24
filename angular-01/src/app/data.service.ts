import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { map } from 'rxjs/operators';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class DataService {
  items: any[] = [];

  constructor(private http: HttpClient) {}

  salvarSaludo(saludador: string[]){
    this.http.post<any>("https://db-tw-92bd2-default-rtdb.firebaseio.com/saludador.json",
    saludador)
    .subscribe(
      response => {console.log("Respuesta del servidor " + response.name)},
      error => console.log("Error del servidor " + error)
    );
  }

  // mostrarSaludador(){
  //   this.http.get("https://db-tw-92bd2-default-rtdb.firebaseio.com/saludador.json").pipe(
  //     map((responseData: any) => {
  //       const items = [];

  //       for(const key in responseData){
  //         if(responseData.hasOwnProperty(key)){
  //           items.push({id: key, ...responseData[key]});
  //         }
  //       }

  //       return items;
  //     })
  //   ).subscribe(items => {console.log(items);});
  // }

  mostrarSaludador(): Observable<any[]>{
    return this.http.get("https://db-tw-92bd2-default-rtdb.firebaseio.com/saludador.json").pipe(
      map((responseData: any) => {
        const items = [];

        for(const key in responseData){
          if(responseData.hasOwnProperty(key)){
            items.push({id: key, ...responseData[key]});
          }
        }

        return items;
      })
    )
  }
}