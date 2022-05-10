using System;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using Microsoft.AspNetCore.WebUtilities;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;
//using Newtonsoft.Json;

namespace WebAPIClient
{
    class Program
    {
        private static readonly HttpClient client = new HttpClient();
        private static async Task ProcessRepositories()
        {
            var query = new Dictionary<string, string>()
            { //%2A
                ["from"] = "Budapest",
                ["to"] = "Wien",
                ["date"] = "2022.05.09"
            };
            var uri = QueryHelpers.AddQueryString("https://apiv2.oroszi.net/elvira", query); //building uri

            var streamTask = client.GetStreamAsync(uri);
            var schedule = await JsonSerializer.DeserializeAsync<Schedule>(await streamTask); //deserialize Json

            foreach (var tr in schedule.timetable)
                Console.WriteLine($"Start> {tr.start} Dest> {tr.destination} start time> {tr.starttime} arrival> {tr.destinationtime}");

        }

        static async Task Main(string[] args)
        {
            await ProcessRepositories();

        }
    }
}

