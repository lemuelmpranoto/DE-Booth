import tweepy
from tweepy.auth import OAuthHandler
def tweet():
    #Update these keys
    consumer_key='Vor0kQTc1J8LNh7HG38T9q0mG'
    consumer_secret='JqksbMfpso0VQ16dNbd4Z6WMU9bvo4VO1REUt4lTU6WgIKz8k8'
    access_token='959468100813930496-75VfXpgig32ZekLiMEuyQw0i6TdUrLA'
    access_secret='IOoxMS5cbR4cy9ZUnRKUIrGDR8lgKE0c4F9KNcOA2IZu4'


    auth = OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_secret)
    api = tweepy.API(auth)

    media_list = []
    response = api.media_upload('../391_Images/filtered_image.jpg')
    media_list.append(response.media_id_string)
    api.update_status("Here is your filtered photo!", media_ids=media_list)

