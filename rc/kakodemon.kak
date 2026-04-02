provide-module kakodemon %{

declare-option str-list kakodemon_masters

hook -group kakodemon global ClientCreate .* %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit
        [ -z "$kak_client_env_KAKOD_CLIENT_ID" ] && exit

        export KAKOD_ID=$kak_client_env_KAKOD_ID

        kakod -p rename-session $kak_session
        kakod -p rename-client $kak_client_env_KAKOD_CLIENT_ID $kak_client
    }
}

hook -group kakodemon global ClientRenamed .* %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit
        [ -z "$kak_client_env_KAKOD_CLIENT_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p rename-client $kak_client_env_KAKOD_CLIENT_ID $kak_client
    }
}

hook -group kakodemon global SessionRenamed .* %{
    evaluate-commands -client * %{
        nop %sh{
            [ -z "$kak_client_env_KAKOD_ID" ] && exit

            KAKOD_ID=$kak_client_env_KAKOD_ID \
            kakod -p rename-session $kak_session
        }
     }
 }

define-command -docstring '
kakodemon-new-client [<commands>]: create a new Kakoune client in Kakodemon
' -params .. kakodemon-new-client %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p new-client "$@"
    }
}

complete-command -menu kakodemon-new-client command

define-command -docstring "
kakodemon-focus [<kakoune_client>]: focus a given client's pane in Kakodemon
If no client is passed, then the current client is used
" -params 0..1 kakodemon-focus %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p focus ${1:-$kak_client}
    }
}

complete-command -menu kakodemon-focus client

define-command -docstring "
kakodemon-layout <layout>: select layouting algorithm for Kakodemon panes
" -params 1 kakodemon-layout %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p set-layout $1
    }
}

complete-command -menu kakodemon-layout shell-script-candidates %{ printf 'tall\nwide\nfull\n' }

alias global new kakodemon-new-client
alias global focus kakodemon-focus

}

hook -group windowing global ClientCreate .* %{
    evaluate-commands %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        printf "%s\n" "require-module kakodemon"
        printf "%s\n" "set-option global windowing_module kakodemon"
    }
}
