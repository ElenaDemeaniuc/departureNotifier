using System;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using Microsoft.AspNetCore.WebUtilities;


namespace WebAPIClient
{
    class Program
    {
        private static readonly HttpClient client = new HttpClient();
        private static async Task ProcessRepositories()
        {
            client.DefaultRequestHeaders.Accept.Clear();
            //client.DefaultRequestHeaders.Accept.Add(
            //    new MediaTypeWithQualityHeaderValue("application/vnd.github.v3+json"));
            //client.DefaultRequestHeaders.Add("User-Agent", ".NET Foundation Repository Reporter");

            var query = new Dictionary<string, string>()
            {
                ["from"] = "Kelenfold",
                ["to"] = "Erd",
                ["date"] = "2022.05.02"
            };

            var uri = QueryHelpers.AddQueryString("https://apiv2.oroszi.net/elvira", query);


            var stringTask = client.GetStringAsync(uri);

            var msg = await stringTask;
            Console.Write(msg);
        }

        static async Task Main(string[] args)
        {
            await ProcessRepositories();

        }
    }
}

