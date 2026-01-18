hook -group kakodemon global ClientCreate .* %{
    nop %sh{
        [ -z "$KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p rename-session $kak_session
    }
}

hook -group kakodemon global SessionRenamed .* %{
    evaluate-commands -client * %{
        nop %sh{
            [ -z "$KAKOD_ID" ] && exit

            KAKOD_ID=$kak_client_env_KAKOD_ID \
            kakod -p rename-session $kak_session
        }
    }
}
