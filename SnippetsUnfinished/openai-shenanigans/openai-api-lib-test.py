import openai

from dotenv import dotenv_values
key = dotenv_values(".env")["OPENAI_API_KEY"]
openai.api_key = key


context = "no context"
question = "what are the steps to cooking spaghetti? spaghetti are a thin type of pasta noodle; I want a tasty recipe that involves tomatos; give me at 5 step recipe"

print(f'asking\n`{context}`\n`{question}`\nwith key\n`{key}`')
response = openai.Completion.create(
    prompt=f"Answer the question based on the context below, and if the question can't be answered based on the context, say \"I don't know\"\n\nContext: {context}\n\n---\n\nQuestion: {question}\nAnswer:",
    temperature=0.5,
    max_tokens=1800,
    top_p=1,
    frequency_penalty=0,
    presence_penalty=0,
    # stop=stop_sequence,
    model="text-davinci-003",
)
resp_text = response["choices"][0]["text"].strip()
print(f"respone\n{response}\n-------------\n{resp_text}")